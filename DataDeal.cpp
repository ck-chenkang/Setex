#include "DataDeal.h"


DataDeal::DataDeal() {


}

vector<typeAndIndex> DataDeal::getTypeAndIndexList(fileRead* fileReadInfo) {

	std::vector<std::string> list;

	typeAndIndex typeIndex;
	vector<typeAndIndex> typeIndexList;

	std::string path = "Config\\";

	vector<int> indexVector;


	list = { "InfoWord", "InfoDoubleWord", "InfoBit", "FunctionInitMarker",
	"FunctionActivationMarker", "FunctionQuitMarker", "ValueParameter",
	"ActualValueParameter", "Alarms", "Alarms2", "ControlMarker",
	"ControlWords", "ControllerParameter", "Timers", "MachineConstant",
	"BatchParameter", "BatchParameterDS737", "BatchParameterNextBatch",
		"MaintenanceCounter", "ActionButtons"
	};

	auto typeInfo = fileReadInfo->typeRead();

	for (auto i : typeInfo) {
		for (auto j : list) {
			if (i == j) {
				std::string path = "Config\\";
				path = path + i + ".txt";
				indexVector = fileReadInfo->readIndex(path);

				for (auto index : indexVector) {

					if (i == "InfoWord" || i == "ControlWords" || i == "MaintenanceCounter") {
						if (index < 1 || index > 200) continue;
					}

					if (i == "InfoDoubleWord" || i == "InfoBit" || i == "Alarms" ||
						i == "Timers" || i == "MachineConstant") {
						if (index < 1 || index > 500) continue;
						
					}

					if (i == "FunctionInitMarker" || i == "FunctionActivationMarker" ||
						i == "FunctionQuitMarker" || i == "ValueParameter" ||
						i == "ActualValueParameter" || i == "ControlMarker") {
						if (index < 1 || index > 1000) continue;
					}

					if (i == "Alarms2") {
						if (index < 1001 || index > 2000) continue;
					}

					if (i == "ControllerParameter") {
						if (index < 1001 || index > 4000) continue;
					}

					if (i == "BatchParameter" || i == "BatchParameterDS737" ||
						i == "BatchParameterNextBatch") {
						if (index < 1 || index > 50) continue;
					}

					if (i == "ActionButtons") {
						if (index < 401 || index > 1000) continue;
					}

					typeIndex.type = i;
					typeIndex.index = index;
					typeIndex.content = 0;
					typeIndex.doubleContent = 0;
					typeIndex.online = false;
					typeIndexList.push_back(typeIndex);
				}
			}
		}
	}
	return typeIndexList;
}



vector<typeAndIndex> DataDeal::getData(setexApi* setexApiData, fileRead* fileReadInfo) {

	vector<typeAndIndex> typeIndexList = getTypeAndIndexList(fileReadInfo);
	vector<typeAndIndex> List;


	vector<int> data;
	vector<int> index;

	int times = 0;

	for (auto i : typeIndexList) {

		//����11����int���͵�
		if (i.type == "InfoWord") {
			int value;
			int result = setexApiData->getInfoWord(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "ValueParameter") {
			int value;
			int result = setexApiData->getValueParameter(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "ActualValueParameter") {
			int value;
			int result = setexApiData->getActualValueParameter(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}
		
		if (i.type == "ControlWords") {
			int value;
			int result = setexApiData->getControlWords(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "ControllerParameter") {
			int value;
			int result = setexApiData->getControllerParameter(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "Timers") {
			int value;
			int result = setexApiData->getTimers(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "MachineConstant") {
			int value;
			int result = setexApiData->getMachineConstant(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "BatchParameter") {
			int value;
			int result = setexApiData->getBatchParameter(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "BatchParameterDS737") {
			int value;
			int result = setexApiData->getBatchParameterDS737(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "BatchParameterNextBatch") {
			int value;
			int result = setexApiData->getBatchParameterNextBatch(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		if (i.type == "MaintenanceCounter") {
			int value;
			int result = setexApiData->getMaintenanceCounter(i.index, value);
			if (result == 0) {
				i.content = value;
			}
			else {
				times++;
			}
		}

		//����1����unsigned���͵�
		if (i.type == "InfoDoubleWord" ) {
			unsigned value;
			int result = setexApiData->getInfoDoubleWord(i.index, value);
			if (result == 0) {
				i.doubleContent = value;
			}
			else {
				times++;
			}
		}

		//����8����bool���͵�
		if (i.type == "InfoBit") {
			bool value;
			int result = setexApiData->getInfoBit(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "FunctionInitMarker") {
			bool value;
			int result = setexApiData->getFunctionInitMarker(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "FunctionActivationMarker") {
			bool value;
			int result = setexApiData->getFunctionActivationMarker(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "FunctionQuitMarker") {
			bool value;
			int result = setexApiData->getFunctionQuitMarker(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "Alarms") {
			bool value;
			int result = setexApiData->getAlarms(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "Alarms2") {
			bool value;
			int result = setexApiData->getAlarms(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "ControlMarker") {
			bool value;
			int result = setexApiData->getControlMarker(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}

		if (i.type == "ActionButtons") {
			bool value;
			int result = setexApiData->getActionButtons(i.index, value);
			if (result == 0) {
				i.content = (int)value;
			}
			else {
				times++;
			}
		}
		if (times) {
			i.online = false;
		}
		else
		{
			i.online = true;
		}
		List.push_back(i);
	}

	return List;
}

DataDeal::~DataDeal() {

}