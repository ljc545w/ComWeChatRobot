#include "pch.h"

#define CheckLoginOffset 0x2366538

#define SwitchQrcodeLoginCall1Offset 0x372AA0
#define SwitchQrcodeLoginCall2Offset 0x5177D0

#define SaveQrcodeImageHookOffset 0x2815DA
#define SaveQrcodeImageNextCallOffset 0x76F660

static DWORD SaveQrcodeImageHookAddr;
static DWORD SaveQrcodeImageNextCall;
static DWORD SaveQrcodeImageJmpBackAddress;

static bool SaveQrcodeImageHooked = false;
static char SaveQrcodeImageOldAsmCode[5] = {0};
static DWORD SignalThreadId = 0;

struct QrcodeStruct
{
    unsigned char *image = NULL;
    int size = 0;
    void update(unsigned char *buf, int len)
    {
        this->free();
        this->image = new unsigned char[len + 1];
        memcpy(this->image, buf, len);
        this->size = len;
    }
    void free()
    {
        if (this->image != NULL)
        {
            delete[] this->image;
            this->image = NULL;
        }
        this->size = 0;
    }
    ~QrcodeStruct()
    {
        this->free();
    }
};

static unique_ptr<QrcodeStruct> qc(new QrcodeStruct);

BOOL isWxLogin()
{
    DWORD CheckLoginAddr = GetWeChatWinBase() + CheckLoginOffset;
    return *(BOOL *)CheckLoginAddr;
}

void SaveQrcodeImage(unsigned char *src, int size)
{
    qc->update(src, size);
    SIGNAL(SignalThreadId, WM_WAIT_HOOK_DATA);
}

_declspec(naked) void dealQrcodeImage()
{
    __asm {
        pushad;
        pushfd;
        push dword ptr[eax + 4];
        push dword ptr[eax];
        call SaveQrcodeImage;
        add esp, 0x8;
        popfd;
        popad;
        call SaveQrcodeImageNextCall;
        jmp SaveQrcodeImageJmpBackAddress;
    }
}

void HookQrcodeImage()
{
    if (SaveQrcodeImageHooked)
        return;
    DWORD WeChatWinBase = GetWeChatWinBase();
    SaveQrcodeImageHookAddr = WeChatWinBase + SaveQrcodeImageHookOffset;
    SaveQrcodeImageNextCall = WeChatWinBase + SaveQrcodeImageNextCallOffset;
    SaveQrcodeImageJmpBackAddress = SaveQrcodeImageHookAddr + 0x5;
    HookAnyAddress(SaveQrcodeImageHookAddr, (LPVOID)dealQrcodeImage, SaveQrcodeImageOldAsmCode);
    SaveQrcodeImageHooked = true;
}

void UnHookQrcodeImage()
{
    if (!SaveQrcodeImageHooked)
        return;
    UnHookAnyAddress(SaveQrcodeImageHookAddr, SaveQrcodeImageOldAsmCode);
    SaveQrcodeImageHooked = false;
}

BOOL __stdcall SwitchToQrcodeLogin()
{
    qc->free();
    DWORD WeChatWinBase = GetWeChatWinBase();
    DWORD SwitchQrcodeLoginCall1 = WeChatWinBase + SwitchQrcodeLoginCall1Offset;
    DWORD SwitchQrcodeLoginCall2 = WeChatWinBase + SwitchQrcodeLoginCall2Offset;
    int isSuccess = 0;
    __asm {
        pushad;
        pushfd;
        call SwitchQrcodeLoginCall1;
        mov ecx,eax;
        call SwitchQrcodeLoginCall2;
        movzx eax,al;
        mov isSuccess,eax;
        popfd;
        popad;
    }
    return isSuccess == 1;
}

#ifndef USE_SOCKET
DWORD GetQrcodeImageRemote()
{
    if (isWxLogin())
        return 0;
    if (!SaveQrcodeImageHooked)
        HookQrcodeImage();
    WxSignal sg(WM_WAIT_HOOK_DATA, SignalThreadId);
    SwitchToQrcodeLogin();
    sg.wait(5000);
    return (DWORD)qc.get();
}
#else
BYTE *__stdcall GetQrcodeImage(int &size)
{
    if (isWxLogin())
        return NULL;
    if (!SaveQrcodeImageHooked)
        HookQrcodeImage();
    WxSignal sg(WM_WAIT_HOOK_DATA, SignalThreadId);
    SignalThreadId = sg.GetThreadId();
    SwitchToQrcodeLogin();
    sg.wait(5000);
    size = qc->size;
    if (qc->size == 0)
        return NULL;
    return qc->image;
}
#endif
