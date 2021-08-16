#include <iostream>
#include <fstream>
#include <vector>

#include "setexApi.h"
#include "dataBase.h"
#include "fileRead.h"
#include "DataDeal.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

using namespace std;

std::string sql(dataBase* database, fileRead* fileReadInfo);
std::string sqlDeal(std::string str1, std::string str2, fileRead* fileReadInfo);
std::vector<std::string> insertSql(DeviceInfo deviceInfo, vector<typeAndIndex> typeIndexList, dataBase* database);
std::vector<std::string> updateSql(DeviceInfo deviceInfo, vector<typeAndIndex> typeIndexList, dataBase* database);


struct Param {
    DeviceInfo i;
    std::vector<bool> onlineList;
    std::vector<setexApi*> setexApiListOnline;
    int times;
};


void* thread(void *);
dataBase* database = NULL;
fileRead* fileReadInfo = NULL;
pthread_t th[13];
pthread_mutex_t mut;
//隐藏windows 终端显示界面
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main()
{
    using namespace std;
    std::cout << "进入主程序!\n";

    //初始化数据库和文件信息
    database = new dataBase();
    fileReadInfo = new fileRead();

    //创建数据库表
    std::string sqlStr, str;
    sqlStr = sql(database, fileReadInfo);
    //std:; cout << sqlStr << std::endl;
    //database->exeSql(sqlStr, fileReadInfo);


    //根据设备信息，更新数据库
    std::vector<DeviceInfo> deviceInfo;
    deviceInfo = fileReadInfo->deviceInfoRead();


    std::string insertsql;

    std::vector<setexApi*> setexApiListOnline;

    std::vector<DeviceInfo> setexListOffLine;

    //存放设备在线和离线信息
    std::vector<bool> onlineList;


    for (auto i : deviceInfo) {

        bool online = false;
        setexApi* setexConnect = new setexApi();
        int ch[10] = { 0,1,0,0,1,0,0,1,1,1 };

        int result = 0;

        //online = database->judgeOnline(i.name, fileReadInfo);

        result = setexConnect->initialize(i.ip.data());

        if ( result == -1) {        
            setexApiListOnline.push_back(setexConnect);
            onlineList.push_back(false);
            continue;
        }
        else {
            onlineList.push_back(true);
        }
        setexApiListOnline.push_back(setexConnect);


        std::cout << "result:" << result << endl;


        setexConnect->getPictureLayers(ch);
        setexConnect->setPictureLayers(ch); 


        DataDeal* dataDeal = new DataDeal();

        vector<typeAndIndex> typeIndexList;


        typeIndexList = dataDeal->getData(setexConnect, fileReadInfo);
        insertsql = insertSql(i, typeIndexList, database).at(0);

        //database->exeSql(insertsql, fileReadInfo);

        //std::cout << "执行完了插入设备表和初始化设备连接" << endl;
        std::cout << "执行完了初始化设备连接" << endl;


        delete dataDeal;
    }
    
    int deviceNumber = (int)deviceInfo.size();


    Param param1;

    pthread_mutex_init(&mut, NULL);
    int index = 0;
    for (auto i : deviceInfo) {
        std::cout << "开始了" << endl;
        param1.i = i;
        param1.onlineList = onlineList;
        param1.setexApiListOnline = setexApiListOnline;
        param1.times = index;       
       
        int res = pthread_create(&th[index], NULL, thread, &param1);
         index++;
        if (res != -1) {
            cout << "成功创建了线程" << index << endl;
        }
    }
    while (true)
    {
        Sleep(1000);

    }
    
    return 0;
}

std::string sql(dataBase* database, fileRead* fileReadInfo) {
    database->getInfo();

    auto typeInfo = fileReadInfo->typeRead();
        
    std::string sql;
    std::vector<std::string> list;
    list = { "InfoWord", "InfoDoubleWord", "InfoBit", "FunctionInitMarker",
    "FunctionActivationMarker", "FunctionQuitMarker", "ValueParameter",
    "ActualValueParameter", "Alarms", "Alarms2", "ControlMarker",
    "ControlWords", "ControllerParameter", "Timers", "MachineConstant",
    "BatchParameter", "BatchParameterDS737", "BatchParameterNextBatch",
        "MaintenanceCounter", "ActionButtons"
    };

    sql = "CREATE TABLE " + database->dataBaseInfo.TABLE + " ( Name  varchar(4) not null, ";
    std::string midStr;

    //i是从配置文件里读出来的
    for (auto i : typeInfo) {
        for (auto j : list) {
            if (i == j) {
                midStr = sqlDeal(i, j, fileReadInfo);
                sql += midStr;
            }
        }
    }
    sql += " PRIMARY KEY (Name) )";
    

    return sql;
}

std::string sqlDeal(std::string str1 ,std::string str2, fileRead* fileReadInfo) {
    
    std::string path = "Config\\";
    vector<int> indexVector;

    std::string str;
    if (str1 == str2) {
        std::string infoStr1 = str2;
        std::string infoStr2 = " int,";
        std::string infoStr3 = " bigint,";

        path = path + str1 + ".txt";
        indexVector = fileReadInfo->readIndex(path);

        for (auto index : indexVector) {

            if (str1 == "InfoWord" || str1 == "ControlWords" || str1 == "MaintenanceCounter") {
                if (index < 1 || index > 200) continue;
            }

            if (str1 == "InfoDoubleWord" || str1 == "InfoBit" || str1 == "Alarms" ||
                str1 == "Timers" || str1 == "MachineConstant" ) {
                if (index < 1 || index > 500) continue;
            }

            if (str1 == "FunctionInitMarker" || str1 == "FunctionActivationMarker" ||
                str1 == "FunctionQuitMarker" || str1 == "ValueParameter" || 
                str1 == "ActualValueParameter" || str1 == "ControlMarker" ) {
                if (index < 1 || index > 1000) continue;
            }

            if (str1 == "Alarms2" ) {
                if (index < 1001 || index > 2000) continue;
            }

            if (str1 == "ControllerParameter") {
                if (index < 1001 || index > 4000) continue;
            }

            if (str1 == "BatchParameter" || str1 == "BatchParameterDS737" ||
                str1 == "BatchParameterNextBatch") {
                if (index < 1 || index > 50) continue;
            }

            if (str1 == "ActionButtons") {
                if (index < 401 || index > 1000) continue;
            }

            //
            if (str1 == "InfoDoubleWord") {
                str += infoStr1;
                str += std::to_string(index);
                str += infoStr3;
                continue;
            }

            str += infoStr1;
            str += std::to_string(index);
            str += infoStr2;
        }
    }
    return str;
}

std::vector<std::string> insertSql(DeviceInfo deviceInfo, vector<typeAndIndex> typeIndexList, dataBase* database) {
    std::vector<std::string> strlist;


    //这里的10主要是应对，std::string字符串太小，装不下拼接的字符串问题，把它们拆开
    for (int j = 0; j < (typeIndexList.size()/10 + 1); j++) {
        int k = 0;

        std::string sql = "";
        sql += "INSERT INTO " + database->dataBaseInfo.TABLE + " (";
        //" (LastName, Address) VALUES ('Wilson', 'Champs-Elysees')";
        auto test = typeIndexList.size();
        for (auto i : typeIndexList) {
            
            if ( ( j )*10 <= k  && k < (j + 1)*10 ) {
                sql += i.type;
                sql += std::to_string(i.index);
                sql += ", ";
            }
            k++;
        }


        k = 0;

        sql += "Name) VALUES (";

        for (auto i : typeIndexList) {
            if ((j) * 10 <= k && k < (j + 1) * 10) {
                if (i.type == "InfoDoubleWord") {

                    sql += std::to_string(i.doubleContent);
                    sql += ", ";
                }
                else
                {
                    sql += std::to_string(i.content);
                    sql += ", ";
                }
            }
            k++;
        }
        sql += "'";

        sql += deviceInfo.name;
        sql += "'";
        sql  += ")";

        strlist.push_back(sql);

    }
    
    return strlist;
}

std::vector<std::string> updateSql(DeviceInfo deviceInfo, vector<typeAndIndex> typeIndexList, dataBase* database) {
    std::vector<std::string> strlist;


    //这里的10主要是应对，std::string字符串太小，装不下拼接的字符串问题，把它们拆开
    for (int j = 0; j < (typeIndexList.size() / 10 + 1); j++) {
        int k = 0;

        std::string sql = "";
        sql += "UPDATE " + database->dataBaseInfo.TABLE + " SET ";
        //UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing'
        //WHERE LastName = 'Wilson'
        auto test = typeIndexList.size();
        for (auto i : typeIndexList) {

            if ((j) * 10 <= k && k < (j + 1) * 10) {
                if (i.type == "InfoDoubleWord") {

                    sql += i.type;
                    sql += std::to_string(i.index);
                    sql += " = ";
                    sql += std::to_string(i.doubleContent);

                    if ( k == ((j + 1) * 10 - 1)) {

                    }
                    else {
                        if (k == (typeIndexList.size() - 1)) {

                        }
                        else {
                            sql += ", ";

                        }
                    }

                }
                else
                {
                    sql += i.type;
                    sql += std::to_string(i.index);
                    sql += " = ";
                    sql += std::to_string(i.content);

                    if (k == ((j + 1) * 10 - 1)) {
                        
                    }
                    else {
                        if (k == (typeIndexList.size() - 1)) {

                        }
                        else {
                            sql += ", ";

                        }
                    }
                }
            }
            k++;
        }

        sql += " WHERE ";
        sql += "Name = ";

        sql += "'";

        sql += deviceInfo.name;
        sql += "'";

        strlist.push_back(sql);

    }

    return strlist;
}


void *thread(void* param){
    pthread_mutex_lock(&mut);
    Param param1 = *(Param*)param;
    int index = param1.times;
    pthread_mutex_unlock(&mut);
    while (true)
    {
        Sleep(30000);
        pthread_mutex_lock(&mut);
        
        cout << "线程： " << index << "正在运行" << endl;       
        if (param1.onlineList.at(index)) {
                std::cout << "设备在线" << endl;
                DataDeal* dataDeal = new DataDeal();

                vector<typeAndIndex> typeIndexList;


               
                typeIndexList = dataDeal->getData(param1.setexApiListOnline.at(index), fileReadInfo);
                
                //判断获取数据情况，由此判断设备是否在线，设备要是不在线的话，就把它的标志位置为不在线
                int count = 0;
                for (auto k : typeIndexList) {
                    if (k.online == false) {
                        count++;
                    }
                }

                if (count == typeIndexList.size()) {
                    param1.onlineList.at(index) = false;
                    continue;
                }

                std::vector<std::string> list;

                list = updateSql(param1.i, typeIndexList, database);

                for (auto j : list) {
                    database->exeSql(j, fileReadInfo);
                }
                
                delete dataDeal;
            }
            else
            {
                //这边用来针对不在线的设备，做一个循环判断，要是在线的话，就把它加进去
                std::cout << "设备不在线" << endl;
               
                    int ch[10] = { 0,1,0,0,1,0,0,1,1,1 };

                    int result = 0;

               
                    delete param1.setexApiListOnline.at(index);
                    param1.setexApiListOnline.at(index) = new setexApi();
                    result = param1.setexApiListOnline.at(index)->initialize(param1.i.ip.data());
                    std::cout << "result:" << result << endl;
                    
                    if (result == -1) {
                        param1.onlineList.at(index) = false;
                        continue;
                    }
                    else {
                        param1.onlineList.at(index) = true;
                    }
                    
                    param1.setexApiListOnline.at(index)->getPictureLayers(ch);
                    param1.setexApiListOnline.at(index)->setPictureLayers(ch);

            }
        pthread_mutex_unlock(&mut);
    }    
}





