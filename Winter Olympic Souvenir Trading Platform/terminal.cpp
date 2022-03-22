#include "terminal.h"
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#pragma warning(disable:4996)
using namespace std;

int getOperationCode()
{
	std::wstring codeString;
	int codeInt;
	std::wcout << L"输入操作：";
	std::wcin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::wcout << L"不正确的输入\n";
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

string wstring2string(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

wstring string2wstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}