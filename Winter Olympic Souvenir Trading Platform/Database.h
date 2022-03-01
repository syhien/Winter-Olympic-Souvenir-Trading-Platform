#pragma once
#include <vector>
#include <map>
#include <string>

class Database
{
public:
	Database(std::vector<std::pair<std::string, std::string> > inputFiles = { {"commodity","commodity.txt"},{"order","order.txt"},{"user","user.txt"}
		});
	std::vector<std::vector<std::pair<std::string, std::wstring> > > perform(std::string command);
private:
	std::map<std::string, std::vector<std::vector<std::pair<std::string, std::wstring> > > > __table;
	std::map<std::string, std::vector<std::string> > __columnOfTable;
	std::vector<std::pair<std::string, std::string> > __tableFiles;
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __select(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __insert(std::istringstream& stream);
	std::vector<std::vector<std::pair<std::string, std::wstring> > > __update(std::istringstream& stream);
	void __save();
};

