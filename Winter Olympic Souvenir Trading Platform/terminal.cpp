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
	std::cout << "Input operation code：";
	std::cin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::cout << "Wrong input\n";
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