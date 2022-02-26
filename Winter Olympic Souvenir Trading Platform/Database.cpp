#include "Database.h"
#include <algorithm>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <typeinfo>
#include <iomanip>
#include <ctime>
#include <ranges>
#include <Windows.h>
#pragma warning(disable:4996)
using namespace std;

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

Database::Database(std::vector<std::pair<std::string, std::string>> inputFiles)
{
	setlocale(LC_ALL, "chs");
	__columnOfTable["commodity"] = { "itemID", "name","price","count","description", "sellerID", "time", "state" };
	__columnOfTable["order"] = { "orderID","itemID","price","count", "time", "sellerID", "buyerID" };
	__columnOfTable["user"] = { "userID", "name","password","contact","address","wallet","state" };
	for (auto& i : inputFiles) {
		auto tableName = i.first;
		auto tablePath = i.second;
		try
		{
			wifstream fin(tablePath, ios::in);
			fin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			if (!fin)
				throw "No selected file called " + tablePath;
			wstring line;
			getline(fin, line);
			while (getline(fin, line)) {
				wistringstream lineStream(line);
				wstring tmpValue;
				int count = 0;
				std::vector<std::pair<std::string, std::wstring> > newLine;
				while (getline(lineStream, tmpValue, L',')) {
					newLine.push_back({ __columnOfTable[tableName][count],tmpValue });
					count++;
				}
				__table[tableName].push_back(newLine);
			}
			fin.close();
		}
		catch (const std::exception&)
		{
			__table[tableName] = std::vector<std::vector<std::pair<std::string, std::wstring> > >();
		}
	}
}

std::vector<std::vector<std::pair<std::string, std::wstring> > > Database::perform(std::string command)
{
	istringstream commandStream(command);
	string instruction;
	commandStream >> instruction;
	if (instruction == "SELECT") {
		return __select(commandStream);
	}
	else if (instruction == "INSERT") {
		return __insert(commandStream);
	}
	else if (instruction == "UPDATE") {
		return __update(commandStream);
	}
	else {
		throw "No selected command called " + instruction;
	}
	return std::vector<std::vector<std::pair<std::string, std::wstring> > >();
}

std::vector<std::vector<std::pair<std::string, std::wstring> > > Database::__select(std::istringstream& stream)
{
	string assertString;
	stream >> assertString;
	if (assertString != "*") {
		throw "Wrong command format.";
	}
	stream >> assertString;
	if (assertString != "FROM") {
		throw "Wrong command format.";
	}
	string tableName;
	stream >> tableName;
	if (__table.find(tableName) == __table.end()) {
		throw "No selected table named " + tableName;
	}
	if (!(stream >> assertString) or assertString.empty()) {
		// select ALL
		return __table[tableName];
	}
	else if (assertString == "WHERE") {
		string column;
		stream >> column;
		stream >> assertString;
		if (assertString != "CONTAINS") {
			throw "Wrong command format.";
		}
		string value;
		stream >> value;
		if ((stream >> assertString) and !assertString.empty()) {
			throw "Wrong command format.";
		}
		std::vector<std::vector<std::pair<std::string, std::wstring> > > result;
		for (auto i : __table[tableName]) {
			for (auto j : i | views::filter([column](pair<string, wstring> k) {return k.first == column; })) {
				if (j.second.find(string2wstring(value)) != wstring::npos)
					result.push_back(i);
			}
		}
		return result;
	}
	else {
		throw "Wrong command format.";
	}
	return std::vector<std::vector<std::pair<std::string, std::wstring> > >();
}

std::vector<std::vector<std::pair<std::string, std::wstring>>> Database::__insert(std::istringstream& stream)
{
	return std::vector<std::vector<std::pair<std::string, std::wstring>>>();
}

std::vector<std::vector<std::pair<std::string, std::wstring>>> Database::__update(std::istringstream& stream)
{
	return std::vector<std::vector<std::pair<std::string, std::wstring>>>();
}
