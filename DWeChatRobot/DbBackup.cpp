#include "pch.h"

BOOL SQLite3_Backup_Init_Patched = FALSE;

/*
* �ⲿ����ʱ���ݵĲ�������
* DbHandle��Ҫ���ݵ����ݿ���
* BackupFile�����ݵı���λ��
*/
#ifndef USE_SOCKET
struct BackupStruct {
	DWORD DbHandle;
	char* BackupFile;
};
#endif

/*
* ���ݿⱸ�ݺ���
* return��int�����쳣����`0`�����쳣���ط�0ֵ
*/
int __cdecl backupDb(
	DWORD pDb,					/* Database to back up */
	const char* zFilename,      /* Name of file to back up to */
	DWORD myMain,
	int address_sqlite3_open,
	int address_sqlite3_backup_init,
	int address_sqlite3_backup_step,
	int address_sqlite3_backup_remaining,
	int address_sqlite3_backup_pagecount,
	int address_sqlite3_sleep,
	int address_sqlite3_backup_finish,
	int address_sqlite3_errcode,
	int address_sqlite3_close,
	void(*xProgress)(int, int)  /* Progress function to invoke */
) {
	int rc;                      /* Function return code */
	DWORD pFile = 0;             /* Database connection opened on zFilename */
	DWORD pBackup = 0;			 /* Backup handle used to copy data */
	Sqlite3_open p_Sqlite3_open = (Sqlite3_open)address_sqlite3_open;
	Sqlite3_backup_init p_Sqlite3_backup_init = (Sqlite3_backup_init)address_sqlite3_backup_init;
	Sqlite3_backup_step p_Sqlite3_backup_step = (Sqlite3_backup_step)address_sqlite3_backup_step;
	Sqlite3_backup_remaining p_Sqlite3_backup_remaining = (Sqlite3_backup_remaining)address_sqlite3_backup_remaining;
	Sqlite3_backup_pagecount p_Sqlite3_backup_pagecount = (Sqlite3_backup_pagecount)address_sqlite3_backup_pagecount;
	Sqlite3_sleep p_Sqlite3_sleep = (Sqlite3_sleep)address_sqlite3_sleep;
	Sqlite3_backup_finish p_Sqlite3_backup_finish = (Sqlite3_backup_finish)address_sqlite3_backup_finish;
	Sqlite3_errcode p_Sqlite3_errcode = (Sqlite3_errcode)address_sqlite3_errcode;
	Sqlite3_close p_Sqlite3_close = (Sqlite3_close)address_sqlite3_close;

	rc = p_Sqlite3_open((const char*)zFilename, &pFile);

	if (rc == SQLITE_OK) {
		pBackup = p_Sqlite3_backup_init(pFile, (const char*)myMain, pDb, (const char*)myMain);
		if (pBackup) {
			do {
				rc = p_Sqlite3_backup_step(pBackup, 5);
				xProgress(
					p_Sqlite3_backup_remaining(pBackup),
					p_Sqlite3_backup_pagecount(pBackup)
				);
				if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
					p_Sqlite3_sleep(50);
				}
			} while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

			(void)p_Sqlite3_backup_finish(pBackup);
		}
		rc = p_Sqlite3_errcode(pFile);
	}
	(void)p_Sqlite3_close(pFile);
	return rc;
}

/*
* �ƹ��������ݿⱸ������
*/
VOID PatchSQLite3_Backup_Init() {
	if (SQLite3_Backup_Init_Patched)
		return;
	// patch�����ָ��ƹ�`backup is not supported with encrypted databases`
	DWORD address_sqlite3_backup_init_patch_offset = OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_INIT_ADDRESS + 0x52);
	DWORD patchAddress = GetWeChatWinBase() + address_sqlite3_backup_init_patch_offset;
	const int nopLen = 22;
	BYTE nopData[nopLen];
	for (int i = 0; i < nopLen; i++) {
		nopData[i] = 0x90;
	}
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)patchAddress, nopData, nopLen, 0);
	SQLite3_Backup_Init_Patched = TRUE;
	return;
}

/*
* ���ݻص�����
*/
void XProgress(int a, int b)
{
#ifdef _DEBUG
	printf("���ݽ���: %d/%d\n", b - a, b);
#endif
	return;
}

/*
* ���ݿ����߱������
* DbHandle��Ҫ���ݵ����ݿ���
* BackupFile�����ݱ���λ��
* return��int�����쳣����`0`�����쳣���ط�0ֵ
*/
int BackupSQLiteDB(DWORD DbHandle,const char* BackupFile)
{
	DWORD wxBaseAddress = GetWeChatWinBase();
	PatchSQLite3_Backup_Init();
#ifdef _DEBUG
	cout << "��ʼ����,�ļ�������: " << BackupFile << endl;
#endif
	DWORD address_sqlite3_open = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_OPEN_ADDRESS);
	DWORD address_sqlite3_backup_init = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_INIT_ADDRESS);
	DWORD address_sqlite3_backup_step = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_STEP_ADDRESS);
	DWORD address_sqlite3_sleep = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_SLEEP_ADDRESS);
	DWORD address_sqlite3_backup_finish = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_FINISH_ADDRESS);
	DWORD address_sqlite3_close = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_CLOSE_ADDRESS);
	DWORD address_sqlite3_backup_remaining = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_REMAINING_ADDRESS);
	DWORD address_sqlite3_backup_pagecount = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_BACKUP_PAGECOUNT_ADDRESS);
	DWORD address_sqlite3_errcode = wxBaseAddress + OffsetFromIdaAddr(IDA_SQLITE3_ERRCODE_ADDRESS);
	const char* myMain = "main";
	int rc = backupDb(
		DbHandle,
		BackupFile,
		(DWORD)myMain,
		address_sqlite3_open,
		address_sqlite3_backup_init,
		address_sqlite3_backup_step,
		address_sqlite3_backup_remaining,
		address_sqlite3_backup_pagecount,
		address_sqlite3_sleep,
		address_sqlite3_backup_finish,
		address_sqlite3_errcode,
		address_sqlite3_close,
		XProgress);
#ifdef _DEBUG
	cout << "�������: " << BackupFile << endl;
#endif
	return rc;
}

/*
* ���ⲿ���õ����ݿ����߱��ݽӿ�
* lpParameter��`BackupStruct`���ͽṹ��ָ��
* return��int�����쳣����`0`�����쳣���ط�0ֵ
*/
#ifndef USE_SOCKET
int BackupSQLiteDBRemote(LPVOID lpParameter) {
	BackupStruct* param = (BackupStruct*)lpParameter;
	int rc = BackupSQLiteDB(param->DbHandle,(const char*)param->BackupFile);
	return rc;
}
#endif