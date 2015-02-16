// SqlLite.cpp : main project file.
//
// Usage: %s DATABASE SQL-STATEMENT
// SqlLite.exe ..\test.db "select * from tb11"

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "sqlite3.h"

using namespace System;
using namespace std;

void Pause()
{
	cout << "Press Enter to continue...";
	cout.flush();
	cin.ignore(10000, '\n');
	return;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv)
{
	sqlite3 *db;
	sqlite3_stmt *res;

	char *zErrMsg = 0;
	int rc;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return(1);
	}
	rc = sqlite3_open(argv[1], &db);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);

	if (rc != SQLITE_OK) {

		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	rc = sqlite3_step(res);

	if (rc == SQLITE_ROW) {
		printf("%s  %s\n", sqlite3_column_name(res, 0), sqlite3_column_text(res, 0));
	}

	sqlite3_finalize(res);

	sqlite3_close(db);
	Pause();
	return 0;
}