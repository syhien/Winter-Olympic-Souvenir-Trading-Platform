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
#pragma warning(disable:4996)
using namespace std;

Database::Database()
{
	setlocale(LC_ALL, "chs");
	__columnOfTable["commodity"] = { "itemID", "name","price","count","description", "sellerID", "time", "state" };
	__commodity.clear();
	ifstream fin("commodity.txt", ios::in);
	if (fin) {
		string tmp, tmp1;
		fin >> tmp;
		while (fin >> tmp) {
			Commodity commodity;
			stringstream line(tmp);
			getline(line, commodity.itemID, ',');
			getline(line, commodity.name, ',');
			getline(line, tmp1, ',');
			commodity.price = stod(tmp1);
			getline(line, tmp1, ',');
			commodity.count = stoi(tmp1);
			getline(line, commodity.description, ',');
			getline(line, commodity.sellerID, ',');
			getline(line, tmp1, ',');
			istringstream timeStream(tmp1);
			timeStream >> get_time(&commodity.time, "%Y-%m-%d");
			getline(line, commodity.state, ',');
			__commodity.push_back(commodity);
		}
	}
	fin.close();
	fin.clear();

	__order.clear();
	__columnOfTable["order"] = { "orderID","itemID","price","count", "time", "sellerID", "buyerID" };
	fin.open("order.txt", ios::in);
	if (fin) {
		string tmp, tmp1;
		fin >> tmp;
		while (fin >> tmp) {
			Order order;
			stringstream line(tmp);
			getline(line, order.orderID, ',');
			getline(line, order.itemID, ',');
			getline(line, tmp1, ',');
			order.price = stod(tmp1);
			getline(line, tmp1, ',');
			order.count = stoi(tmp1);
			getline(line, tmp1, ',');
			istringstream timeStream(tmp1);
			timeStream >> get_time(&order.time, "%Y-%m-%d");
			getline(line, order.sellerID, ',');
			getline(line, order.buyerID, ',');
			__order.push_back(order);
		}
	}
	fin.close();
	fin.clear();

	__user.clear();
	__columnOfTable["user"] = { "userID", "name","password","contact","address","wallet","state" };
	fin.open("user.txt", ios::in);
	if (fin) {
		string tmp, tmp1;
		fin >> tmp;
		while (fin >> tmp) {
			User user;
			stringstream line(tmp);
			getline(line, user.userID, ',');
			getline(line, user.name, ',');
			getline(line, user.password, ',');
			getline(line, user.contact, ',');
			getline(line, user.address, ',');
			getline(line, tmp1, ',');
			user.wallet = stod(tmp1);
			getline(line, tmp1, ',');
			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			user.state = converter.from_bytes(tmp1) == L"Õý³£";
			__user.push_back(user);
		}
	}
	fin.close();
	fin.clear();
}

std::vector<std::map<std::string, std::string>> Database::perform(std::string command)
{
	istringstream stream(command);
	string instructionName;
	stream >> instructionName;
	if (instructionName == "INSERT") {
		string tmp;
		stream >> tmp;
		if (tmp != "INTO") {
			throw "Wrong command format.";
		}
		string tableName;
		stream >> tableName;
		if (tableName != "commodity" and tableName != "order" and tableName != "user") {
			throw "No selected table named " + tableName;
		}
		stream >> tmp;
		if (tmp != "VALUES") {
			throw "Wrong command format.";
		}
		stream >> tmp;
		if (tmp.front() != '(' or tmp.back() != ')') {
			throw "Wrong command format.";
		}
		tmp.erase(tmp.begin());
		tmp.pop_back();
		istringstream valuesStream(tmp);
		vector<string> newLine;
		string tmpValue;
		while (getline(valuesStream, tmpValue, ',') and !tmpValue.empty())
		{
			newLine.push_back(tmpValue);
		}
		if (newLine.size() != __columnOfTable[tableName].size()) {
			throw "Less or too many values.";
		}
		if (tableName == "commodity") {
			Commodity commodity;
			istringstream timeStream(newLine[6]);
			for (int i = 0; i < newLine.size(); i++) {
				switch (i)
				{
				case 0:
					commodity.itemID = newLine[i];
					break;
				case 1:
					commodity.name = newLine[i];
					break;
				case 2:
					try
					{
						commodity.price = stod(newLine[i]);
						break;
					}
					catch (const std::exception&)
					{
						throw "Wrong command format.";
					}
				case 3:
					try
					{
						commodity.count = stoi(newLine[i]);
						break;
					}
					catch (const std::exception&)
					{
						throw "Wrong command format.";
					}
				case 4:
					commodity.description = newLine[i];
					break;
				case 5:
					commodity.sellerID = newLine[i];
					break;
				case 6:
					timeStream >> get_time(&commodity.time, "%Y-%m-%d");
					break;
				case 7:
					commodity.state = newLine[i];
					break;
				default:
					break;
				}
			}
			__commodity.push_back(commodity);
		}
		else if (tableName == "order") {

		}
		else if (tableName == "user") {

		}
		else {
			throw "No selected table named " + tableName;
		}
	}
	else if (instructionName == "UPDATE") {

	}
	else if (instructionName == "SELECT") {
		string tmp;
		stream >> tmp;
		if (tmp != "*") {
			throw "Wrong command format.";
		}
		stream >> tmp;
		if (tmp != "FROM") {
			throw "Wrong command format.";
		}
		string tableName;
		stream >> tableName;
		if (tableName != "commodity" and tableName != "order" and tableName != "user") {
			throw "No selected table named " + tableName;
		}
		stream >> tmp;
		if (tmp.empty() or tmp != "WHERE") { //select ALL
			if (tableName == "commodity") {
				std::vector<std::map<std::string, std::string>> ret;
				map<string, string> line = {
					{"itemID",""},
					{"name",""},
					{"price",""},
					{"count",""},
					{"description",""},
					{"sellerID",""},
					{"time",""},
					{"state",""}
				};
				for (auto& i : __commodity) {
					line["itemID"] = i.itemID;
					line["name"] = i.name;
					line["price"] = to_string(i.price);
					line["count"] = to_string(i.count);
					line["description"] = i.description;
					line["sellerID"] = i.sellerID;
					stringstream tmpStream;
					tmpStream << put_time(&i.time, "%Y-%m-%d");
					line["time"] = tmpStream.str();
					line["state"] = i.state;
					ret.push_back(line);
				}
				return ret;
			}
			else if (tableName == "order") {
				std::vector<std::map<std::string, std::string>> ret;
				map<string, string> line = {
					{"orderID",""},
					{"itemID",""},
					{"price",""},
					{"count",""},
					{"time",""},
					{"sellerID",""},
					{"buyerID",""}
				};
				for (auto& i : __order) {
					line["orderID"] = i.orderID;
					line["itemID"] = i.itemID;
					line["price"] = to_string(i.price);
					line["count"] = to_string(i.count);
					stringstream tmpStream;
					tmpStream << put_time(&i.time, "%Y-%m-%d");
					line["time"] = tmpStream.str();
					line["sellerID"] = i.sellerID;
					line["buyerID"] = i.buyerID;
					ret.push_back(line);
				}
				return ret;
			}
			else if (tableName == "user") {
				std::vector<std::map<std::string, std::string>> ret;
				map<string, string> line = {
					{"userID",""},
					{"name",""},
					{"password",""},
					{"contact",""},
					{"address",""},
					{"wallet",""},
					{"state",""}
				};
				for (auto& i : __user) {
					line["userID"] = i.userID;
					line["name"] = i.name;
					line["password"] = i.password;
					line["contact"] = i.contact;
					line["address"] = i.address;
					line["wallet"] = to_string(i.wallet);
					line["state"] = to_string(i.state);
					ret.push_back(line);
				}
				return ret;
			}
			else {
				throw "No selected table named " + tableName;
			}
		}
		else {
			string column;
			stream >> column;
			if (tableName == "commodity") {

			}
			else if (tableName == "order") {

			}
			else if (tableName == "user") {
				if (find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), column) == __columnOfTable[tableName].end()) {
					throw "No selected colomn named" + column;
				}
				auto columnIt = find(__columnOfTable[tableName].begin(), __columnOfTable[tableName].end(), column);
				int columnPosition = distance(__columnOfTable[tableName].begin(), columnIt);
				
			}
			else {
				throw "No selected table named " + tableName;
			}
		}
	}
	else {
		throw "Unknown command.";
	}
	return std::vector<std::map<std::string, std::string>>();
}
