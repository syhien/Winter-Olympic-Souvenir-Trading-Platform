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
#include "terminal.h"
#pragma warning(disable:4996)
using namespace std;

Database::Database(std::vector<std::pair<std::string, std::string>> inputFiles)
{
	__tableFiles = inputFiles;
	setlocale(LC_ALL, "chs");
	__columnOfTable["commodity"] = { "itemID", "name","price","count","description", "sellerID", "time", "state" };
	__columnOfTable["order"] = { "orderID","itemID","price","count", "time", "sellerID", "buyerID" };
	__columnOfTable["user"] = { "userID", "name","password","contact","address","wallet","state" };
	__columnOfTable["recharge"] = { "userID","money","date" };
	for (auto& i : inputFiles) {
		if (i.first == "commands")
			continue;
		auto tableName = i.first;
		auto tablePath = i.second;
		try
		{
			wifstream fin(tablePath, ios::in);
			fin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			if (!fin)
				throw exception("No selected file.");
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

std::vector<std::vector<std::pair<std::string, std::wstring>>> Database::perform(std::string command, std::string sender, std::string mode)
{
	auto result = __perform(command);
	if (sender == "admin" and mode == "123456")
		return result;
	if (mode == "seller") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != sender; } return true; }), result.end());
		return result;
	}
	if (mode == "buyer") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "buyerID") return wstring2string(j.second) != sender; } return false; }), result.end());
		return result;
	}
	if (mode == "user") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "userID") return wstring2string(j.second) != sender; } return true; }), result.end());
		return result;
	}
	return std::vector<std::vector<std::pair<std::string, std::wstring>>>();
}

std::vector<std::vector<std::pair<std::string, std::wstring> > > Database::__perform(std::string command)
{
	__saveCommand(command);
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
		throw exception("No selected command.");
	}
	return std::vector<std::vector<std::pair<std::string, std::wstring> > >();
}

Database::~Database()
{
	__save();
}

std::vector<std::vector<std::pair<std::string, std::wstring> > > Database::__select(std::istringstream& stream)
{
	string assertString;
	stream >> assertString;
	if (assertString != "*") {
		throw exception("Wrong command format.");
	}
	stream >> assertString;
	if (assertString != "FROM") {
		throw exception("Wrong command format.");
	}
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception("No selected table.");
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
			throw exception("Wrong command format.");
		}
		string value;
		stream >> value;
		if ((stream >> assertString) and !assertString.empty()) {
			throw exception("Wrong command format.");
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
		throw exception("Wrong command format.");
	}
	return std::vector<std::vector<std::pair<std::string, std::wstring> > >();
}

std::vector<std::vector<std::pair<std::string, std::wstring>>> Database::__insert(std::istringstream& stream)
{
	string assertString;
	stream >> assertString;
	if (assertString != "INTO") {
		throw exception("Wrong command format.");
	}
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception("No selected table.");
	}
	stream >> assertString;
	if (assertString != "VALUES") {
		throw exception("Wrong command format.");
	}
	char assertChar;
	stream >> assertChar;
	if (assertChar != '(') {
		throw exception("Wrong command format.");
	}
	string line;
	stream >> line;
	if (line.back() != ')') {
		throw exception("Wrong command format.");
	}
	line.pop_back();
	istringstream lineStream(line);
	std::vector<std::pair<std::string, std::wstring>> newLine;
	for (auto& i : __columnOfTable[tableName]) {
		string newValue;
		getline(lineStream, newValue, ',');
		if (newValue.empty()) {
			throw exception("Wrong command format.");
		}
		newLine.push_back({ i,string2wstring(newValue) });
	}
	__table[tableName].push_back(newLine);
	__save();
	return std::vector<std::vector<std::pair<std::string, std::wstring>>>();
}

std::vector<std::vector<std::pair<std::string, std::wstring>>> Database::__update(std::istringstream& stream)
{
	string assertString;
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception("No selected table.");
	}
	stream >> assertString;
	if (assertString != "SET") {
		throw exception("Wrong command format.");
	}
	string updateColumn, updateValue;
	map<string, wstring> updateDict;
	char assertChar;
	while (stream >> updateColumn and updateColumn != "WHERE" and updateColumn != "HERE") {
		stream >> assertChar;
		if (assertChar != '=') {
			throw exception("Wrong command format.");
		}
		stream >> updateValue;
		stream >> assertChar;
		if (assertChar != ',' and assertChar != 'W') {
			throw exception("Wrong command format.");
		}
		if (find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), updateColumn) == __columnOfTable[tableName].end()) {
			throw exception("No selected column.");
		}
		updateDict[updateColumn] = string2wstring(updateValue);
	}
	if (updateColumn != "WHERE" and updateColumn != "HERE") {
		throw exception("Wrong command format.");
	}
	string column, value;
	stream >> column;
	stream >> assertChar;
	stream >> value;
	if (assertChar != '=') {
		throw exception("Wrong command format.");
	}
	if (find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), column) == __columnOfTable[tableName].end()) {
		throw exception("No selected column.");
	}
	for (auto& i : __table[tableName])
		for (auto& j : i | views::filter([column, value](pair<string, wstring> k) {return k.first == column and k.second == string2wstring(value); }))
			for (auto& l : i)
				if (updateDict.contains(l.first))
					l.second = updateDict[l.first];
	__save();
	return std::vector<std::vector<std::pair<std::string, std::wstring>>>();
}

void Database::__saveCommand(std::string command)
{
	setlocale(LC_ALL, "chs");
	string commandsFile;
	for (auto& i : __tableFiles)
		if (i.first == "commands")
			commandsFile = i.second;
	ofstream fout(commandsFile, ios::out | ios::app);
	time_t t = time(nullptr);
	std::tm tm = *localtime(&t);
	stringstream transTime;
	transTime << put_time(&tm, "%Y-%m-%d %H:%M:%S");
	fout << transTime.str() << ": " << command << endl;
}

void Database::__save()
{
	setlocale(LC_ALL, "chs");
	map<string, wstring> tableTitle = { {"commodity",L"商品ID,名称,价格,数量,描述,卖家ID,上架时间,商品状态"},{"order",L"订单ID,商品ID,交易单价,数量,交易时间,卖家ID,买家ID"},{"user",L"用户ID,用户名,密码,联系方式,地址,钱包余额,用户状态"},{"recharge",L"用户ID,充值金额,充值时间"} };
	for (auto& i : __tableFiles) {
		if (i.first == "commands")
			continue;
		wofstream fout(i.second, ios::out | ios::trunc);
		fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		if (!fout)
			throw exception("Unable to save database.");
		fout << tableTitle[i.first] << endl;
		for (auto& j : __table[i.first]) {
			for (auto& k : j) {
				if (k.first == __columnOfTable[i.first].back())
					fout << k.second << endl;
				else
					fout << k.second << L",";
			}
		}
		fout.close();
	}
}
