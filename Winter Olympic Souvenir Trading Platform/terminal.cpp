#include "terminal.h"
#include <string>
#include <format>
#include <vector>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#pragma warning(disable:4996)
using namespace std;

int getOperationCode()
{
	std::string codeString;
	int codeInt;
	std::cout << "输入操作：";
	std::cin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::cout << "不正确的输入\n";
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