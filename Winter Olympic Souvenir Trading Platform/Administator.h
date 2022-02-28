#pragma once
#include <string>
#include "Database.h"
class Administator
{
public:
	Administator(Database* database);
	bool login(std::string name, std::string password);
	void HomePage();
private:
	Database* __database;
	const std::string __name = "admin";
	const std::string __password = "123456";
	void handleCommodity();
	void handleOrder();
	void handleUser();
};

