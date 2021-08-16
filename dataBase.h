#pragma once
#include "fileRead.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
using namespace std;

class dataBase
{
public:
	DataBaseInfo dataBaseInfo;
	dataBase();
	void getInfo();
	void exeSql(std::string sql, fileRead* fileReadInfo);
	void StringToWstring(std::wstring& szDst, std::string str);

	bool judgeOnline(std::string name, fileRead* fileReadInfo);


};

