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
using namespace std;

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
			ifstream fin(tablePath, ios::in);
			if (!fin)
				throw "No selected file called " + tablePath;
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
