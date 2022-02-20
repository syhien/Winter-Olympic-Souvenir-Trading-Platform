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
#pragma warning(disable:4996)
using namespace std;

Database::Database()
{
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
			user.state = converter.from_bytes(tmp1) == L"Õı³£";
			__user.push_back(user);
		}
	}
	fin.close();
	fin.clear();
}
