#pragma once
#include <iostream>
#include <string>
#include <vector>

//���ݿ��ȡ��Ϣ��Ŵ�
struct DataBaseInfo {
	std::string SERVER;
	std::string USERID;
	std::string PWD;
	std::string DATABASE;
	std::string TABLE;
};

struct DeviceInfo {
	std::string name;
	std::string ip;
};


class fileRead
{
public:
	fileRead();

	std::vector<int> readIndex(std::string path);

	DataBaseInfo dataBaseInfoRead();

	std::vector<DeviceInfo> deviceInfoRead();

	std::vector<std::string> typeRead();

	~fileRead();

};

