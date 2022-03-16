#pragma once
#include <vector>
#include <map>
#include <string>

class Database
{
public:
	Database(std::vector<std::pair<std::string, std::string> > inputFiles = { {"commodity","commodity.txt"},{"order","order.txt"},{"user","user.txt"},{"recharge","recharge.txt"},{"commands","commands.txt"}
		});
	std::vector<std::vector<std::pair<std::string, std::wstring> > > perform(std::string command, std::string sender, std::string mode);
	~Database();
private:
	std::map<std::string, std::vector<std::vector<std::pair<std::string, std::wstring> > > > __table;
	std::map<std::string, std::vector<std::string> > __columnOfTable;
	std::vector<std::pair<std::string, std::string> > __tableFiles;
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __select(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __insert(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __update(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __perform(std::string command);
	void __saveCommand(std::string command);
	void __save();
};

