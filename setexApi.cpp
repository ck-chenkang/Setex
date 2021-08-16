#include "setexApi.h"
#include <iostream>
#include <Ws2tcpip.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"WS2_32.lib")

using namespace std;
const int BUFF_MAX = 516;

setexApi::setexApi() {

}

int setexApi::initialize(const char* deviceip) {
	const char* ip = deviceip;
	int port = 50000;

	struct    sockaddr_in     serverSockAddr;
	int       rc;
	WSADATA   data;

	WSAStartup(MAKEWORD(2, 0), &data);

	if ((sockHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return -1;
	}

	memset(&serverSockAddr, 0, sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;

	inet_pton(AF_INET, ip, &(serverSockAddr.sin_addr.s_addr));
	serverSockAddr.sin_port = htons(port);

	rc = connect(sockHandle, (struct sockaddr*)&serverSockAddr, sizeof(serverSockAddr));
	if (rc < 0)
	{
		if (sockHandle >= 0)
		{
			closesocket(sockHandle);
		}
		WSACleanup();
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getInfoWord(int index, int& value) {
	if (index < 1 || index > 200)return -1;

	int     bytesRecv;
	
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x01;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setInfoWord(int index, int value) {
	if (index < 1 || index > 200)return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x01;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;    buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getInfoDoubleWord(int index, unsigned& value) {
	if (index < 1 || index > 500)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x39;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		int value3 = (unsigned char)buffR[22];
		int value4 = (unsigned char)buffR[23];
		value = value1 + value2 * 256 + value3 * 256 * 256 + value4 * 256 * 256 * 256;
		return 0;
	}
}

int setexApi::setInfoDoubleWord(int index, unsigned value) {
	if (index < 1 || index > 500)return -1;
	if (value < 0 || value > 256 * 256 * 256 * 256 - 1) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256 % 256;
	int value3 = value / 256 / 256 % 256;
	int value4 = value / 256 / 256 / 256;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x16;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x39;     buffS[17] = 0x04;     buffS[18] = 0x06;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;    buffS[23] = value2;
	buffS[23] = value3;   buffS[24] = value4;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getInfoBit(int index, bool& value) {
	if (index < 1 || index > 500)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x02;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		return 0;
	}
}

int setexApi::setInfoBit(int index, bool value) {
	if (index < 1 || index > 500)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x02;     buffS[17] = 0x04;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getFunctionInitMarker(int index, bool& value) {
	if (index < 1 || index > 1000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0xf4;     buffS[17] = 0x03;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		return 0;
	}
}

int setexApi::setFunctionInitMarker(int index, bool value) {
	if (index < 1 || index > 1000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf4;     buffS[17] = 0x03;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getFunctionActivationMarker(int index, bool& value) {
	//std::cout << "现在执行读取FA的函数：" << std::endl;
	//std::cout << std::endl << std::endl;
	if (index < 1 || index > 1000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0xf3;     buffS[17] = 0x03;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		//std::cout << "序号：" << index << "值为：";
		//std::cout << "bytesRecv < 0" << std::endl << std::endl;
		
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		//std::cout << "序号：" << index << "值为：";
		//std::cout << "value: " << value << std::endl << std::endl;
		//std::cout << "执行完了读取FA的函数";
		return 0;
	}
}

int setexApi::setFunctionActivationMarker(int index, bool value) {
	if (index < 1 || index > 1000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf3;     buffS[17] = 0x03;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getFunctionQuitMarker(int index, bool& value) {
	if (index < 1 || index > 1000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0xf5;     buffS[17] = 0x03;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		return 0;
	}
}

int setexApi::setFunctionQuitMarker(int index, bool value) {
	if (index < 1 || index > 1000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf5;     buffS[17] = 0x03;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getValueParameter(int index, int& value) {
	if (index < 1 || index > 1000) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0xf1;     buffS[17] = 0x03;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setValueParameter(int index, int value) {
	if (index < 1 || index > 1000) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf1;     buffS[17] = 0x03;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;    buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getActualValueParameter(int index, int& value) {
	if (index < 1 || index > 1000) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0xf2;     buffS[17] = 0x03;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setActualValueParameter(int index, int value) {
	if (index < 1 || index > 1000) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf2;     buffS[17] = 0x03;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;    buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getAlarms(int index, bool& value) {
	if (index < 1 || index > 500)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0xe2;      buffS[3] = 0x01;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x80;
	buffS[16] = 0x20;     buffS[17] = 0x04;     buffS[18] = 0xd2;     buffS[19] = 0x01;
	buffS[20] = 0x01;     buffS[21] = 0x00;     buffS[22] = 0xf6;     buffS[23] = 0x03;
	buffS[24] = index1;   buffS[24] = index2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[26];
		value = data;
		return 0;
	}
}

int setexApi::setAlarms(int index, bool value) {
	if (index < 1 || index > 500)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0xf6;     buffS[17] = 0x03;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getAlarms2(int index, bool& value) {
	if (index < 1001 || index > 2000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = (index - 1000) % 256;
	int index2 = (index - 1000) / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0xe2;      buffS[3] = 0x01;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x80;
	buffS[16] = 0x20;     buffS[17] = 0x04;     buffS[18] = 0xd2;     buffS[19] = 0x01;
	buffS[20] = 0x01;     buffS[21] = 0x00;     buffS[22] = 0x4f;     buffS[23] = 0x04;
	buffS[24] = index1;   buffS[25] = index2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[26];
		value = data;
		return 0;
	}
}

int setexApi::setAlarms2(int index, bool value) {
	if (index < 1001 || index > 2000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x4f;     buffS[17] = 0x04;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getControlMarker(int index, bool& value) {
	if (index < 1 || index > 1000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		return 0;
	}
}

int setexApi::setControlMarker(int index, bool value) {
	if (index < 1 || index > 1000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getControlWords(int index, int& value) {
	if (index < 1 || index > 200) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x38;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setControlWords(int index, int value) {
	if (index < 1 || index > 200) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x38;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;    buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getControllerParameter(int index, int& value) {
	if (index < 1 || index > 4000) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x04;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setControllerParameter(int index, int value) {
	if (index < 1 || index > 4000) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x04;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getTimers(int index, int& value) {
	if (index < 1 || index > 500) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x03;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setTimers(int index, int value) {
	if (index < 1 || index > 500) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x03;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getMachineConstant(int index, int& value) {
	if (index < 1 || index > 500) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x05;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setMachineConstant(int index, int value) {
	if (index < 1 || index > 500) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x05;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getBatchParameter(int index, int& value) {
	if (index < 1 || index > 50) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x34;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setBatchParameter(int index, int value) {
	if (index < 1 || index > 50) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x34;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getBatchParameterDS737(int index, int& value) {
	if (index < 1 || index > 50) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x36;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setBatchParameterDS737(int index, int value) {
	if (index < 1 || index > 50) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x36;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getBatchParameterNextBatch(int index, int& value) {
	if (index < 1 || index > 50) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x35;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setBatchParameterNextBatch(int index, int value) {
	if (index < 1 || index > 50) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x35;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getMaintenanceCounter(int index, int& value) {
	if (index < 1 || index > 200) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x53;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int value1 = (unsigned char)buffR[20];
		int value2 = (unsigned char)buffR[21];
		value = value2 * 256 + value1;
		return 0;
	}
}

int setexApi::setMaintenanceCounter(int index, int value) {
	if (index < 1 || index > 200) return -1;
	if (value < 0 || value > 32767) return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int value1 = value % 256;
	int value2 = value / 256;
	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x16;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x53;     buffS[17] = 0x04;     buffS[18] = 0x06;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = value1;   buffS[23] = value2;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getActionButtons(int index, bool& value) {
	if (index < 401 || index > 1000)return -1;

	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x12;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x01;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0x02;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = 0x00;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		int data = buffR[20];
		value = data;
		return 0;
	}
}

int setexApi::setActionButtons(int index, bool value) {
	if (index < 401 || index > 1000)return -1;

	int     data;
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	if (value) data = 1;
	else data = 0;

	int index1 = index % 256;
	int index2 = index / 256;

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x13;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x02;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0x03;     buffS[19] = 0x00;
	buffS[20] = index1;   buffS[21] = index2;   buffS[22] = data;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

int setexApi::getPictureLayers(int* ch) {
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0x14;      buffS[3] = 0x00;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x0b;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0x04;     buffS[19] = 0x00;
	buffS[20] = 0x8d;     buffS[21] = 0x00;     buffS[22] = 0x0a;     buffS[23] = 0x00;

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		for (int i = 20; i < 30; i++) {
			ch[i - 20] = buffR[i];
		}
		return 0;
	}
}

int setexApi::setPictureLayers(int* ch) {
	int     bytesRecv;
	char    buffR[BUFF_MAX];
	char    buffS[BUFF_MAX];

	memset(buffS, 0, sizeof(buffS));
	memset(buffR, 0, sizeof(buffR));

	buffS[0] = 0x02;      buffS[1] = 0x00;      buffS[2] = 0xf4;      buffS[3] = 0x01;
	buffS[12] = 0x00;     buffS[13] = 0x00;     buffS[14] = 0x01;     buffS[15] = 0x0c;
	buffS[16] = 0x37;     buffS[17] = 0x04;     buffS[18] = 0xe4;     buffS[19] = 0x01;
	buffS[20] = 0x8d;     buffS[21] = 0x00;     buffS[22] = 0x0a;     buffS[23] = 0x00;

	for (int i = 24; i < 34; i++) {
		buffS[i] = ch[i - 24];
	}

	send(sockHandle, buffS, BUFF_MAX, 0);
	Sleep(10);

	bytesRecv = recv(sockHandle, buffR, BUFF_MAX, 0);
	if (bytesRecv < 0)
	{
		return -1;
	}
	else {
		return 0;
	}
}

setexApi::~setexApi() {
	try {
		closesocket(sockHandle);
	}
	catch (exception e) {		
		std::cout << "setexApi.cpp 析构函数出错" << std::endl;
	}
}

extern "C" void(*_indexChangedCallBack)(int i) = 0;
void setexApi::setNotifyIndexChangeCallBack(void(*indexChangedCallBack)(int i)) {
	_indexChangedCallBack = indexChangedCallBack;
}

