#include "terminal.h"
#include <string>
#include <format>
#include <vector>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#include <Windows.h>
#pragma warning(disable:4996)
using namespace std;

int getOperationCode()
{
	std::string codeString;
	int codeInt;
	std::cout << "���������";
	std::cin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::cout << "����ȷ������\n";
		codeInt = getOperationCode();
	}
	return codeInt;
}

std::string getCurrentTime()
{
	time_t t = time(nullptr);
	std::tm tm = *localtime(&t);
	stringstream transTime;
	transTime << put_time(&tm, "%Y-%m-%d");
	string time = transTime.str();
	return time;
}

string wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

wstring string2wstring(string str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}