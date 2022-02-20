#pragma once
#include <string>

struct User {
	std::string userID;
	std::string name;
	std::string password;
	std::string contact;
	std::string address;
	double wallet;
	bool state;
};