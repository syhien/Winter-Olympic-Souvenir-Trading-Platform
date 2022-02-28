#include "Calculator.h"
#include <map>
#include <stack>
#include <set>
#include <ctype.h>
#include <ranges>
using namespace std;


std::vector<item> Calculator::divide(std::string input)
{
	vector<item> mid;
	const set<char> legalChar = { '+','-','*','/','(',')' };
	for (int i = 0; i < input.size(); i++) {
		if (legalChar.contains(input[i])) {
			mid.push_back({ false,0,input[i] });
		}
		else if (isdigit(input[i])) {
			int len = 1;
			while (i + len < input.size()) {
				if (isdigit(input[i + len]) or input[i + len] == '.') {
					len++;
				}
				else
					break;
			}
			string operand = input.substr(i, len);
			if (operand.front() == '.' or operand.back() == '.') {
				throw exception("Wrong number format.");
			}
			int dotCount = 0;
			for (auto& i : operand | views::filter([](char j) {return j == '.'; }))
				dotCount++;
			if (dotCount > 1) {
				throw exception("Wrong number format.");
			}
			try
			{
				mid.push_back({ true, stod(input.substr(i, len)),' ' });
				i += len - 1;
			}
			catch (const std::exception&)
			{
				throw exception("Wrong number format.");
			}
		}
		else if (input[i] == ' ') {
			continue;
		}
		else {
			throw exception("Unknown char.");
		}
	}
	return mid;
}

void Calculator::check(std::vector<item>& mid)
{
	// test continuous correctness
	for (int i = 0; i < mid.size(); i++) {
		if (mid[i].isOperand) {
			if (i - 1 >= 0 and mid[i - 1].isOperand)
				throw exception("Continuous operands.");
			if (i + 1 < mid.size() and mid[i + 1].isOperand)
				throw exception("Continuous operands.");
		}
		else {
			if (mid[i]._operator == '(') {
				if (i - 1 >= 0 and mid[i - 1].isOperand)
					throw exception("Missing operator.");
				if (i + 1 < mid.size() and !mid[1 + 1].isOperand and mid[i + 1]._operator != '-')
					throw exception("Missing operand.");
			}
			else if (mid[i]._operator == ')') {
				if (i - 1 >= 0 and !mid[i - 1].isOperand)
					throw exception("Missing operand.");
				if (i + 1 < mid.size() and mid[i + 1].isOperand)
					throw exception("Missing operator.");
			}
			else {
				if (i - 1 >= 0 and !mid[i - 1].isOperand)
					throw exception("Wrong format.");
				if (i + 1 < mid.size() and !mid[i + 1].isOperand)
					throw exception("Wrong format.");
			}
		}
	}

	// test parenthesis
	stack<char> checkParenthesis;
	for (auto& i : mid) {
		if (!i.isOperand and i._operator == '(')
			checkParenthesis.push(i._operator);
		else if (!i.isOperand and i._operator == ')') {
			if (checkParenthesis.empty())
				throw exception("Missing paired parentheses.");
			else
				checkParenthesis.pop();
		}
	}
	if (checkParenthesis.size())
		throw exception("Missing paired parentheses.");
}

std::string Calculator::perform(std::string input)
{
    const map<char, int> stackPriority = { {'#',0},{'(',1},{'*',5},{'/',5},{'+',3},{'-',3},{')',6} };
    const map<char, int> comePriority = { {'#',0},{'(',6},{'*',4},{'/',4},{'+',2},{'-',2},{')',1} };
	try
	{
		auto mid = divide(input);
		check(mid);
	}
	catch (const std::exception& e)
	{
		return e.what();
	}
    return std::string();
}
