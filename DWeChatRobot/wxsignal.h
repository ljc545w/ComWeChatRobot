#pragma once
#include <windows.h>

#define SIGNAL(id, uMsg) PostThreadMessage(id, uMsg, 0, 0)

class WxSignal
{
public:
    WxSignal(UINT uMsg, DWORD &dwThread)
    {
        this->hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)this->Pump, (LPVOID)uMsg, 0, &this->threadId);
        dwThread = this->threadId;
    }
    ~WxSignal()
    {
        if (this->hThread)
            CloseHandle(this->hThread);
        SIGNAL(this->threadId, WM_QUIT);
    }
    static void Pump(UINT uMsg)
    {
        MSG msg = {0};
        int ret;
        while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
        {
            if (msg.message == uMsg)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    void wait(int timeout)
    {
        if (this->hThread)
        {
            WaitForSingleObject(this->hThread, timeout);
            PostThreadMessage(this->threadId, WM_QUIT, 0, 0);
            CloseHandle(this->hThread);
            this->hThread = NULL;
        }
    }
    DWORD GetThreadId()
    {
        return this->threadId;
    }

private:
    HANDLE hThread;
    DWORD threadId;
};
