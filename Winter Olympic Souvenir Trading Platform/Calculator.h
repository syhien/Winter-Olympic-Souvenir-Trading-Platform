#pragma once
#include <string>
#include <vector>
struct item
{
	bool isOperand;
	double operand;
	char _operator;
};
class Calculator
{
private:
	std::vector<item> divide(std::string input);
	void check(std::vector<item>& mid);
	std::vector<item> midToBack(std::vector<item>& mid);
public:
	std::string perform(std::string input);
};

