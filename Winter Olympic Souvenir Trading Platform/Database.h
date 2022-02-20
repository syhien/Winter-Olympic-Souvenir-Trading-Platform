#pragma once
#include "commodity.h"
#include "order.h"
#include "user.h"
#include <vector>

class Database
{
public:
	Database();
private:
	std::vector<Commodity> __commodity;
	std::vector<Order> __order;
	std::vector<User> __user;
};

