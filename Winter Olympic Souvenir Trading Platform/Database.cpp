#include "Database.h"
#include <algorithm>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <iomanip>
#include <ctime>
#include <ranges>
#include <stdexcept>
#include <exception>
#include "terminal.h"
#pragma warning(disable:4996)
using namespace std;

Database::Database(std::vector<std::pair<std::string, std::string>> inputFiles)
{
	__tableFiles = inputFiles;
	__columnOfTable["commodity"] = { "commodityID","commodityName","price","number","description","sellerID","addedDate","state" };
	__columnOfTable["order"] = { "orderID","commodityID","unitPrice","number","date","sellerID","buyerID" };
	__columnOfTable["user"] = { "userID","username","password","phoneNumber","address","balance","userState" };
	__columnOfTable["recharge"] = { "userID","money","date" };
	for (auto& i : inputFiles) {
		if (i.first == "commands")
			continue;
		auto tableName = i.first;
		__table[tableName] = std::vector<std::vector<std::pair<std::string, std::string> > >();
		auto tablePath = i.second;
		try
		{
			ifstream fin(tablePath, ios::in);
			if (!fin) {
				throw exception(logic_error("No selected file."));
			}
			string line;
			getline(fin, line);
			while (getline(fin, line)) {
				istringstream lineStream(line);
				string tmpValue;
				int count = 0;
				std::vector<std::pair<std::string, std::string> > newLine;
				while (getline(lineStream, tmpValue, ',')) {
					newLine.push_back({ __columnOfTable[tableName][count],tmpValue });
					count++;
				}
				__table[tableName].push_back(newLine);
			}
			fin.close();
		}
		catch (const std::exception&)
		{
			__table[tableName] = std::vector<std::vector<std::pair<std::string, std::string> > >();
		}
	}
}

std::vector<std::vector<std::pair<std::string, std::string>>> Database::perform(std::string command, std::string sender, std::string mode)
{
	auto result = __perform(command);
	if (sender == "admin" and mode == "123456")
		return result;
	if (mode == "seller") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != sender; } return true; }), result.end());
		return result;
	}
	if (mode == "buyer") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "buyerID") return j.second != sender; } return false; }), result.end());
		return result;
	}
	if (mode == "user") {
		result.erase(remove_if(result.begin(), result.end(), [sender](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "userID") return j.second != sender; } return true; }), result.end());
		return result;
	}
	return std::vector<std::vector<std::pair<std::string, std::string>>>();
}

std::vector<std::vector<std::pair<std::string, std::string> > > Database::__perform(std::string command)
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
		throw exception(logic_error("No selected command."));
	}
	return std::vector<std::vector<std::pair<std::string, std::string> > >();
}

Database::~Database()
{
	__save();
}

std::vector<std::vector<std::pair<std::string, std::string> > > Database::__select(std::istringstream& stream)
{
	string assertString;
	stream >> assertString;
	if (assertString != "*") {
		throw exception(logic_error("Wrong command format."));
	}
	stream >> assertString;
	if (assertString != "FROM") {
		throw exception(logic_error("Wrong command format."));
	}
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception(logic_error("No selected table."));
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
			throw exception(logic_error("Wrong command format."));
		}
		string value;
		stream >> value;
		if ((stream >> assertString) and !assertString.empty()) {
			throw exception(logic_error("Wrong command format."));
		}
		std::vector<std::vector<std::pair<std::string, std::string> > > result;
		for (auto i : __table[tableName]) {
			for (auto j : i | views::filter([column](pair<string, string> k) {return k.first == column; })) {
				if (j.second.find(value) != string::npos)
					result.push_back(i);
			}
		}
		return result;
	}
	else {
		throw exception(logic_error("Wrong command format."));
	}
	return std::vector<std::vector<std::pair<std::string, std::string> > >();
}

std::vector<std::vector<std::pair<std::string, std::string>>> Database::__insert(std::istringstream& stream)
{
	string assertString;
	stream >> assertString;
	if (assertString != "INTO") {
		throw exception(logic_error("Wrong command format."));
	}
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception(logic_error("No selected table."));
	}
	stream >> assertString;
	if (assertString != "VALUES") {
		throw exception(logic_error("Wrong command format."));
	}
	char assertChar;
	stream >> assertChar;
	if (assertChar != '(') {
		throw exception(logic_error("Wrong command format."));
	}
	string line;
	stream >> line;
	if (line.back() != ')') {
		throw exception(logic_error("Wrong command format."));
	}
	line.pop_back();
	istringstream lineStream(line);
	std::vector<std::pair<std::string, std::string>> newLine;
	for (auto& i : __columnOfTable[tableName]) {
		string newValue;
		getline(lineStream, newValue, ',');
		if (newValue.empty()) {
			throw exception(logic_error("Wrong command format."));
		}
		newLine.push_back({ i,newValue });
	}
	__table[tableName].push_back(newLine);
	__save();
	return std::vector<std::vector<std::pair<std::string, std::string>>>();
}

std::vector<std::vector<std::pair<std::string, std::string>>> Database::__update(std::istringstream& stream)
{
	string assertString;
	string tableName;
	stream >> tableName;
	if (!__table.contains(tableName)) {
		throw exception(logic_error("No selected table."));
	}
	stream >> assertString;
	if (assertString != "SET") {
		throw exception(logic_error("Wrong command format."));
	}
	string updateColumn, updateValue;
	map<string, string> updateDict;
	char assertChar;
	while (stream >> updateColumn and updateColumn != "WHERE" and updateColumn != "HERE") {
		stream >> assertChar;
		if (assertChar != '=') {
			throw exception(logic_error("Wrong command format."));
		}
		stream >> updateValue;
		if (updateValue.back() != ',')
			stream >> assertChar;
		else
			assertChar = updateValue.back(), updateValue.pop_back();
		if (assertChar != ',' and assertChar != 'W') {
			throw exception(logic_error("Wrong command format."));
		}
		if (find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), updateColumn) == __columnOfTable[tableName].end()) {
			throw exception(logic_error("No selected column."));
		}
		updateDict[updateColumn] = updateValue;
	}
	if (updateColumn != "WHERE" and updateColumn != "HERE") {
		throw exception(logic_error("Wrong command format."));
	}
	string column, value;
	stream >> column;
	stream >> assertChar;
	stream >> value;
	if (assertChar != '=') {
		throw exception(logic_error("Wrong command format."));
	}
	if (find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), column) == __columnOfTable[tableName].end()) {
		throw exception(logic_error("No selected column."));
	}
	for (auto& i : __table[tableName])
		for (auto& j : i | views::filter([column, value](pair<string, string> k) {return k.first == column and k.second == value; }))
			for (auto& l : i)
				if (updateDict.contains(l.first))
					l.second = updateDict[l.first];
	__save();
	return std::vector<std::vector<std::pair<std::string, std::string>>>();
}

void Database::__saveCommand(std::string command)
{
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
	map<string, string> tableTitle = { {"commodity","commodityID,commodityName,price,number,description,sellerID,addedDate,state"},{"order","orderID,commodityID,unitPrice,number,date,sellerID,buyerID"},{"user","userID,username,password,phoneNumber,address,balance,userState"},{"recharge","userID,money,rechargeDate"} };
	for (auto& i : __tableFiles) {
		if (i.first == "commands")
			continue;
		ofstream fout(i.second, ios::out | ios::trunc);
		if (!fout)
			throw exception(logic_error("Unable to save database."));
		fout << tableTitle[i.first] << endl;
		for (auto& j : __table[i.first]) {
			for (auto& k : j) {
				if (k.first == __columnOfTable[i.first].back())
					fout << k.second << endl;
				else
					fout << k.second << ",";
			}
		}
		fout.close();
	}
}
