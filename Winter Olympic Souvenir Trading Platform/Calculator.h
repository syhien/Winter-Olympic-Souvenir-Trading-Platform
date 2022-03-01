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
	std::vector<item> __divide(std::string input);
	void __check(std::vector<item>& mid);
	std::vector<item> __midToBack(std::vector<item>& mid);
	double __calculate(std::vector<item>& back);
public:
	std::string perform(std::string input);
};

