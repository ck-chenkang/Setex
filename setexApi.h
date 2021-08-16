#pragma once
#include <WinSock2.h>

class setexApi
{
public:
	setexApi();
	
	~setexApi();

	SOCKET  sockHandle;

	int initialize(const char* deviceip);

	int getInfoWord(int index, int& value);
	int setInfoWord(int index, int value);

	int getInfoDoubleWord(int index, unsigned& value);
	int setInfoDoubleWord(int index, unsigned value);

	int getInfoBit(int index, bool& value);
	int setInfoBit(int index, bool value);

	int getFunctionInitMarker(int index, bool& value);
	int setFunctionInitMarker(int index, bool value);

	int getFunctionActivationMarker(int index, bool& value);
	int setFunctionActivationMarker(int index, bool value);

	int getFunctionQuitMarker(int index, bool& value);
	int setFunctionQuitMarker(int index, bool value);

	int getValueParameter(int index, int& value);
	int setValueParameter(int index, int value);

	int getActualValueParameter(int index, int& value);
	int setActualValueParameter(int index, int value);

	int getAlarms(int index, bool& value);
	int setAlarms(int index, bool value);

	int getAlarms2(int index, bool& value);
	int setAlarms2(int index, bool value);

	int getControlMarker(int index, bool& value);
	int setControlMarker(int index, bool value);

	int getControlWords(int index, int& value);
	int setControlWords(int index, int value);

	int getControllerParameter(int index, int& value);
	int setControllerParameter(int index, int value);

	int getTimers(int index, int& value);
	int setTimers(int index, int value);

	int getMachineConstant(int index, int& value);
	int setMachineConstant(int index, int value);

	int getBatchParameter(int index, int& value);
	int setBatchParameter(int index, int value);

	int getBatchParameterDS737(int index, int& value);
	int setBatchParameterDS737(int index, int value);

	int getBatchParameterNextBatch(int index, int& value);
	int setBatchParameterNextBatch(int index, int value);

	int getMaintenanceCounter(int index, int& value);
	int setMaintenanceCounter(int index, int value);

	int getActionButtons(int index, bool& value);
	int setActionButtons(int index, bool value);

	int getPictureLayers(int* ch);
	int setPictureLayers(int* ch);

	void setNotifyIndexChangeCallBack(void(*indexChangedCallBack)(int i));
};

