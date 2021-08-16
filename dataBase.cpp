#include "dataBase.h"
#include <iostream>

dataBase::dataBase() {
    std::cout << "进入数据库类的构造函数" << endl;
}

void dataBase::getInfo() {

	fileRead* dataFileRead = new fileRead();
	dataBaseInfo = dataFileRead->dataBaseInfoRead();
    //auto str = L"SELECT * from AlarmList";

    std::cout << "获取数据库信息成功" << endl;
    return;
}

void dataBase::exeSql(std::string sql, fileRead* fileReadInfo) {
    
    DataBaseInfo dataBaseInfo = fileReadInfo->dataBaseInfoRead();

    std::wstring szDst;

    StringToWstring(szDst, sql);
    SQLWCHAR* statement = (SQLWCHAR*)szDst.c_str();



    //std::string serverIonfo = "DRIVER={SQL Server};SERVER="+ dataBaseInfo.SERVER+", 1433;DATABASE="+ dataBaseInfo.DATABASE+";UID="+ dataBaseInfo.USERID+";PWD="+dataBaseInfo.PWD+";";
    std::string serverIonfo = "DRIVER={SQL Server};SERVER=" + dataBaseInfo.SERVER + ";DATABASE=" + dataBaseInfo.DATABASE + ";UID=" + dataBaseInfo.USERID + ";PWD=" + dataBaseInfo.PWD + ";";


    std::wstring server;
    StringToWstring(server, serverIonfo);

    SQLWCHAR* serverConnectInfo = (SQLWCHAR*)server.c_str();


#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
    //define handles and variables
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
    //initializations
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;
    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        goto COMPLETED;
    //output
    cout << "Attempting connection to SQL Server...";
    cout << "\n";
    //connect to SQL Server  
    //I am using a trusted connection and port 14808
    //it does not matter if you are using default or named instance
    //just make sure you define the server name and the port
    //You have the option to use a username/password instead of a trusted connection
    //but is more secure to use a trusted connection
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        serverConnectInfo,
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_INVALID_HANDLE:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    case SQL_ERROR:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    default:
        break;
    }
    //if there is a problem connecting then exit application
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        goto COMPLETED;
    //output
    cout << "\n";
    cout << "Executing T-SQL query...";
    cout << "\n";
    //if there is a problem executing the query then exit application
    //else display query result
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)statement, SQL_NTS)) {
        cout << "Error querying SQL Server";
        cout << "\n";
        goto COMPLETED;
    }
    else {
        //declare output variable and pointer
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
            //display query result
            cout << "\nQuery Result:\n\n";
            cout << sqlVersion << endl;
        }
        std::cout << "执行完了sql语句" << endl;
    }
    //close connection and free resources
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

}

//判断设备有没有在线
bool dataBase::judgeOnline(std::string name , fileRead* fileReadInfo) {
    DataBaseInfo dataBaseInfo = fileReadInfo->dataBaseInfoRead();

    //返回值，用来标定设备是否在线
    bool flag = false;

    std::wstring szDst;
    std::string sql = "select machine, OnlineState FROM MachineStatus where MachineStatus.machine = '" + name + "'";


    StringToWstring(szDst, sql);
    SQLWCHAR* statement = (SQLWCHAR*)szDst.c_str();



    std::string serverIonfo = "DRIVER={SQL Server};SERVER=" + dataBaseInfo.SERVER + ", 1433;DATABASE=" + dataBaseInfo.DATABASE + ";UID=" + dataBaseInfo.USERID + ";PWD=" + dataBaseInfo.PWD + ";";

    std::wstring server;
    StringToWstring(server, serverIonfo);

    SQLWCHAR* serverConnectInfo = (SQLWCHAR*)server.c_str();


#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
    //define handles and variables
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
    //initializations
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;
    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        goto COMPLETED;
    //output
    cout << "Attempting connection to SQL Server...";
    cout << "\n";
    //connect to SQL Server  
    //I am using a trusted connection and port 14808
    //it does not matter if you are using default or named instance
    //just make sure you define the server name and the port
    //You have the option to use a username/password instead of a trusted connection
    //but is more secure to use a trusted connection
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        serverConnectInfo,
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_INVALID_HANDLE:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    case SQL_ERROR:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    default:
        break;
    }
    //if there is a problem connecting then exit application
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        goto COMPLETED;
    //output
    cout << "\n";
    cout << "Executing T-SQL query...";
    cout << "\n";
    //if there is a problem executing the query then exit application
    //else display query result
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)statement, SQL_NTS)) {
        cout << "Error querying SQL Server";
        cout << "\n";
        goto COMPLETED;
    }
    else {
        //declare output variable and pointer
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStmtHandle, 2, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
            //display query result
            cout << "\nQuery Result:\n\n";
            cout << sqlVersion << endl;

            //类型转换 SQLCHAR to std::string 
            std::string s;
            s = std::string((const char*)sqlVersion);

            if (s ==  "1") {
                flag = true;
            }
        }
        std::cout << "执行完了sql语句" << endl;
    }
    //close connection and free resources
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

    return flag;

}


void dataBase::StringToWstring(std::wstring& szDst, std::string str)
{
    std::string temp = str;
    int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
    wchar_t* wszUtf8 = new wchar_t[len + 1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
    szDst = wszUtf8;
    std::wstring r = wszUtf8;
    delete[] wszUtf8;
}