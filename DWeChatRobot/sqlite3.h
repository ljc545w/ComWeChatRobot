#pragma once
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

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#define SQLITE_TEXT     3

#define IDA_BASE								0x10000000
#define IDA_SQLITE3_EXEC_ADDRESS				0x1141BDF0

#define IDA_SQLITE3_OPEN_ADDRESS				0x11450520
#define IDA_SQLITE3_BACKUP_INIT_ADDRESS			0x113E19B0
#define IDA_SQLITE3_BACKUP_STEP_ADDRESS			0x113E1DB0
#define IDA_SQLITE3_SLEEP_ADDRESS				0x11450D60
#define IDA_SQLITE3_BACKUP_FINISH_ADDRESS		0x113E23F0
#define IDA_SQLITE3_CLOSE_ADDRESS				0x1144D910
#define IDA_SQLITE3_BACKUP_REMAINING_ADDRESS	0x113E24F0
#define IDA_SQLITE3_BACKUP_PAGECOUNT_ADDRESS	0x113E2500
#define IDA_SQLITE3_ERRCODE_ADDRESS				0x1144F1C0


#define IDA_SQLITE3_PREPARE_ADDRESS				0x114227F0
#define IDA_SQLITE3_PREPARE_V2_ADDRESS			0x11422810
#define IDA_SQLITE3_STEP_ADDRESS				0x113EA780
#define IDA_SQLITE3_COLUMN_COUNT_ADDRESS		0x113EACD0
#define IDA_SQLITE3_COLUMN_NAME_ADDRESS			0x113EB630
#define IDA_SQLITE3_COLUMN_TYPE_ADDRESS			0x113EB470
#define IDA_SQLITE3_COLUMN_BLOB_ADDRESS			0x113EAD10
#define IDA_SQLITE3_COLUMN_BYTES_ADDRESS		0x113EADD0
#define IDA_SQLITE3_FINALIZE_ADDRESS			0x113E9730

typedef int(__cdecl* Sqlite3_open)(const char*, DWORD*);
typedef DWORD(__cdecl* Sqlite3_backup_init)(DWORD, const char*, DWORD, const char*);
typedef int(__cdecl* Sqlite3_backup_step)(DWORD, int);
typedef int(__cdecl* Sqlite3_backup_remaining)(DWORD);
typedef int(__cdecl* Sqlite3_backup_pagecount)(DWORD);
typedef int(__cdecl* Sqlite3_sleep)(int);
typedef int(__cdecl* Sqlite3_backup_finish)(DWORD);
typedef int(__cdecl* Sqlite3_errcode)(DWORD);
typedef int(__cdecl* Sqlite3_close)(DWORD);

typedef int(__cdecl* Sqlite3_prepare)(DWORD, const char*, int, DWORD**, int);
typedef int(__cdecl* Sqlite3_step)(DWORD*);
typedef int(__cdecl* Sqlite3_column_count)(DWORD*);
typedef const char*(__cdecl* Sqlite3_column_name)(DWORD*, int);
typedef int(__cdecl* Sqlite3_column_type)(DWORD*, int);
typedef const void*(__cdecl* Sqlite3_column_blob)(DWORD*, int);
typedef int(__cdecl* Sqlite3_column_bytes)(DWORD*, int);
typedef int(__cdecl* Sqlite3_finalize)(DWORD*);

DWORD OffsetFromIdaAddr(DWORD idaAddr);