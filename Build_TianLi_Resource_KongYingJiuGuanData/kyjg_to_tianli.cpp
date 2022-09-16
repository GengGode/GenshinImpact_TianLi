#include "kyjg_to_tianli.h"

int to_tianli()
{
	sqlite3* db;
	sqlite3_open("../GenshinImpact_TianLi_Resource/resource/build_KongYingJiuGuanData.sqlite", &db);
	sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA temp_store = MEMORY", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA cache_size = 10000", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA count_changes = OFF", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA page_size = 4096", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA locking_mode = EXCLUSIVE", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA auto_vacuum = FULL", NULL, NULL, NULL);
	sqlite3_exec(db, "PRAGMA encoding = \"UTF-8\"", NULL, NULL, NULL);
	
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT * FROM kongyingjiuguan", -1, &stmt, NULL);
	
	



	sqlite3_close(db);
	return 0;
}