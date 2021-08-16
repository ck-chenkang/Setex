#include "fileRead.h"
#include <iostream>
#include <fstream>
#include <string>


fileRead::fileRead() {


}

fileRead::~fileRead() {

}

DataBaseInfo fileRead::dataBaseInfoRead() {

	DataBaseInfo dataBaseInfo;
	std::string midStr;

	std::ifstream readFile;
	readFile.open("Config\\DataBaseInfo.txt");

	while (!readFile.eof())
	{
		readFile >> midStr;
		if (midStr.find("SERVER") != std::string::npos) {
			auto index = midStr.find_first_of('=');
			dataBaseInfo.SERVER = midStr.substr(index + 1);
		}

		if (midStr.find("USERID") != std::string::npos) {
			auto index = midStr.find_first_of('=');
			dataBaseInfo.USERID = midStr.substr(index + 1);
		}

		if (midStr.find("PWD") != std::string::npos) {
			auto index = midStr.find_first_of('=');
			dataBaseInfo.PWD = midStr.substr(index + 1);
		}

		if (midStr.find("DATABASE") != std::string::npos) {
			auto index = midStr.find_first_of('=');
			dataBaseInfo.DATABASE = midStr.substr(index + 1);
		}

		if (midStr.find("TABLE") != std::string::npos) {
			auto index = midStr.find_first_of('=');
			dataBaseInfo.TABLE = midStr.substr(index + 1);
		}
	}
	readFile.close();

	return dataBaseInfo;
}

std::vector<int> fileRead::readIndex(std::string path) {

	std::vector<int> indexVector;
	int indexContent;
	std::ifstream readFile;
	readFile.open(path);

	while (!readFile.eof())
	{	
		std::string midContent;
		readFile >> midContent;
		//×Ö·û´®×ª»»³Éint
		try
		{
			indexContent = std::stoi(midContent);
		}
		catch (const std::exception&)
		{
			break;
		}
		indexVector.push_back(indexContent);
	}
	readFile.close();
	return indexVector;
}

std::vector<DeviceInfo> fileRead::deviceInfoRead() {
	
	DeviceInfo deviceInfo;
	std::vector<DeviceInfo> deviceVector;
	std::string midStr;

	std::ifstream readFile;
	readFile.open("Config\\DeviceInfo.txt");


	while (!readFile.eof())
	{
		readFile >> midStr;
		auto index = midStr.find_first_of('=');
		deviceInfo.name = midStr.substr(0, index);
		deviceInfo.ip = midStr.substr(index + 1);
		deviceVector.push_back(deviceInfo);
	}
	readFile.close();


	return deviceVector;
}

std::vector<std::string> fileRead::typeRead() {

	std::vector<std::string> data;
	std::string midStr;
	std::ifstream readFile;

	readFile.open("Config\\TypeOption.txt");

	while (!readFile.eof())
	{
		readFile >> midStr;
		data.push_back(midStr);
	}

	readFile.close();
	return data;

}