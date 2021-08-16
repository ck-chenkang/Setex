#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "setexApi.h"
#include "fileRead.h"

using namespace std;

struct typeAndIndex
{
	std::string type;
	int index;
	int content;
	unsigned doubleContent;
	bool online;
};

class DataDeal
{
public:

	DataDeal();

	~DataDeal();

	vector<typeAndIndex> getData(setexApi * setexApiData, fileRead* fileReadInfo);

private :
	vector<typeAndIndex> getTypeAndIndexList(fileRead* fileReadInfo);

};

