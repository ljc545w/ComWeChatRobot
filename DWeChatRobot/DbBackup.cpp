#include "pch.h"

#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Internal use only */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Not used */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

#define IDA_BASE 0x10000000
BOOL SQLite3_Backup_Init_Patched = FALSE;
DWORD lpAddressBackupDB = 0x0;

typedef int(__cdecl* Sqlite3_open)(const char*, DWORD*);
typedef DWORD(__cdecl* Sqlite3_backup_init)(DWORD, const char*, DWORD, const char*);
typedef int(__cdecl* Sqlite3_backup_step)(DWORD, int);
typedef int(__cdecl* Sqlite3_backup_remaining)(DWORD);
typedef int(__cdecl* Sqlite3_backup_pagecount)(DWORD);
typedef int(__cdecl* Sqlite3_sleep)(int);
typedef int(__cdecl* Sqlite3_backup_finish)(DWORD);
typedef int(__cdecl* Sqlite3_errcode)(DWORD);
typedef int(__cdecl* Sqlite3_close)(DWORD);

DWORD OffsetFromIdaAddr(DWORD idaAddr) {
	return idaAddr - IDA_BASE;
}

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

VOID PatchSQLite3_Backup_Init() {
	if (SQLite3_Backup_Init_Patched)
		return;
	// patch掉这块指令，绕过`backup is not supported with encrypted databases`
	DWORD address_sqlite3_backup_init_patch_offset = OffsetFromIdaAddr(0x1131C110 + 0x52);
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

void XProgress(int a, int b)
{
#ifdef _DEBUG
	printf("备份进度: %d/%d\n", b - a, b);
#endif
	return;
}

int BackupSQLiteDB(DWORD DbHandle,const char* BackupFile)
{
	DWORD wxBaseAddress = GetWeChatWinBase();
	PatchSQLite3_Backup_Init();
#ifdef _DEBUG
	cout << "开始备份,文件保存至: " << BackupFile << endl;
#endif
	DWORD address_sqlite3_open = wxBaseAddress + OffsetFromIdaAddr(0x1138ACD0);
	DWORD address_sqlite3_backup_init = wxBaseAddress + OffsetFromIdaAddr(0x1131C110);
	DWORD address_sqlite3_backup_step = wxBaseAddress + OffsetFromIdaAddr(0x1131C510);
	DWORD address_sqlite3_sleep = wxBaseAddress + OffsetFromIdaAddr(0x1138B510);
	DWORD address_sqlite3_backup_finish = wxBaseAddress + OffsetFromIdaAddr(0x1131CB50);
	DWORD address_sqlite3_close = wxBaseAddress + OffsetFromIdaAddr(0x113880A0);
	DWORD address_sqlite3_backup_remaining = wxBaseAddress + OffsetFromIdaAddr(0x1131CC50);
	DWORD address_sqlite3_backup_pagecount = wxBaseAddress + OffsetFromIdaAddr(0x1131CC60);
	DWORD address_sqlite3_errcode = wxBaseAddress + OffsetFromIdaAddr(0x11389970);
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
	cout << "备份完成: " << BackupFile << endl;
#endif
	return rc;
}

BOOL BackupSQLiteDBRemote(LPVOID lpParameter) {
	BackupStruct* param = (BackupStruct*)lpParameter;
	int rc = BackupSQLiteDB(param->DbHandle,(const char*)param->BackupFile);
	return rc;
}