#pragma once
#include "commodity.h"
#include "order.h"
#include "user.h"
#include <vector>
#include <map>
#include <string>

class Database
{
public:
	Database(std::vector<std::pair<std::string, std::string> > inputFiles = { {"commodity","commodity.txt"},{"order","order.txt"},{"user","user.txt"}
		});
	std::vector<std::vector<std::pair<std::string, std::string> > > perform(std::string command);
private:
	std::map<std::string, std::vector<std::vector<std::pair<std::string, std::string> > > > __table;
	std::map<std::string, std::vector<std::string> > __columnOfTable;
	std::vector<std::vector<std::pair<std::string, std::string> > > __select(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::string> > > __insert(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::string> > > __update(std::istringstream& stream);
};

