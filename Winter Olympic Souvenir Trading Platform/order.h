#pragma once
#include <string>
#include <iomanip>

struct Order {
	std::string orderID;
	std::string itemID;
	double price;
	int count;
	std::tm time;
	std::string sellerID;
	std::string buyerID;
};