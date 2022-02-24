#include "pch.h"
#include <iostream>
#include "../Winter Olympic Souvenir Trading Platform/Database.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(selectAll, select) {
	Database db;
	auto result = db.perform("SELECT * FROM commodity");
	for (auto& i : result) {
		for (auto& j : i) {
			std::cout << "key=" + j.first + ", value=" + j.second << std::endl;
		}
	}
}