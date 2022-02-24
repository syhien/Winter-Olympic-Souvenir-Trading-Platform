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
	Database();
	std::vector< std::map<std::string, std::string> > perform(std::string command);
private:
	std::vector<Commodity> __commodity;
	std::vector<Order> __order;
	std::vector<User> __user;
	std::map<std::string, std::vector<std::string> > __columnOfTable;
};

