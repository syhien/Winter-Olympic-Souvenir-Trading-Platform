#pragma once
#include <string>
#include <iomanip>
struct Commodity {
	std::string itemID;
	std::string name;
	double price;
	int count;
	std::string description;
	std::string sellerID;
	std::tm time;
	std::string state;
};