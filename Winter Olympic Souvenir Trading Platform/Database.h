#pragma once
#include <vector>
#include <map>
#include <string>

class Database
{
public:
	Database(std::vector<std::pair<std::string, std::string> > inputFiles = { {"commodity","commodity.txt"},{"order","order.txt"},{"user","user.txt"},{"recharge","recharge.txt"},{"commands","commands.txt"},{"blacklist","blacklist.txt"}
		});
	std::vector<std::vector<std::pair<std::string, std::string> > > perform(std::string command, std::string sender, std::string mode);
	~Database();
private:
	std::map<std::string, std::vector<std::vector<std::pair<std::string, std::string> > > > __table;
	std::map<std::string, std::vector<std::string> > __columnOfTable;
	std::vector<std::pair<std::string, std::string> > __tableFiles;
	std::vector<std::vector<std::pair<std::string, std::string> > > __select(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::string> > > __insert(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::string> > > __update(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::string> > > __perform(std::string command);
	void __saveCommand(std::string command);
	void __save();
};

