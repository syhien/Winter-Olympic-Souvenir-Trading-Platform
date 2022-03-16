#include "Calculator.h"
#include <map>
#include <stack>
#include <set>
#include <ctype.h>
#include <ranges>
#include <format>
using namespace std;


std::vector<item> Calculator::__divide(std::string input)
{
	vector<item> mid;
	const set<char> legalChar = { '+','-','*','/','(',')' };
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == '.') {
			throw exception("Wrong number format.");
		}
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

void Calculator::__check(std::vector<item>& mid)
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
				if (i + 1 < mid.size() and !mid[i + 1].isOperand and mid[i + 1]._operator != '-' and mid[i + 1]._operator != '(')
					throw exception("Missing operand.");
			}
			else if (mid[i]._operator == ')') {
				if (i - 1 >= 0 and !mid[i - 1].isOperand and mid[i - 1]._operator != ')')
					throw exception("Missing operand.");
				if (i + 1 < mid.size() and mid[i + 1].isOperand)
					throw exception("Missing operator.");
			}
			else {
				if (i - 1 >= 0 and !mid[i - 1].isOperand and mid[i - 1]._operator != '(' and mid[i - 1]._operator != ')')
					throw exception("Wrong format.");
				if (i + 1 < mid.size() and !mid[i + 1].isOperand and mid[i + 1]._operator != '(' and mid[i + 1]._operator != ')')
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

std::vector<item> Calculator::__midToBack(std::vector<item>& mid)
{
	// begin with '-'
	vector<item> newMid;
	if (!mid.front().isOperand and mid.front()._operator == '-')
		newMid.push_back({ true,0,' ' });
	for (auto i = mid.begin(); i != mid.end(); i++) {
		newMid.push_back(*i);
		if (i->isOperand == false and i->_operator == '(' and (i + 1) != mid.end() and (i + 1)->isOperand == false and (i + 1)->_operator == '-')
			newMid.push_back({ true,0,' ' });
	}

	map<char, int> stackPriority = { {'#',0},{'(',1},{'*',5},{'/',5},{'+',3},{'-',3},{')',6} };
	map<char, int> comePriority = { {'#',0},{'(',6},{'*',4},{'/',4},{'+',2},{'-',2},{')',1} };

	newMid.push_back({ false,0,'#' });
	vector<item> back;
	stack<char> operatorStack;
	operatorStack.push('#');
	for (auto& i : newMid) {
		if (i.isOperand)
			back.push_back(i);
		else {
			while (comePriority[i._operator] < stackPriority[operatorStack.top()]) {
				back.push_back({ false,0,operatorStack.top() });
				operatorStack.pop();
			}
			if (comePriority[i._operator] == stackPriority[operatorStack.top()]) {
				operatorStack.pop();
				continue;
			}
			operatorStack.push(i._operator);
		}
	}
	return back;
}

double Calculator::__calculate(std::vector<item>& back)
{
	stack<double> result;
	for (auto& i : back) {
		if (i.isOperand)
			result.push(i.operand);
		else {
			double a, b;
			b = result.top();
			result.pop();
			a = result.top();
			result.pop();
			switch (i._operator)
			{
			case '+':
				result.push(a + b);
				break;
			case '-':
				result.push(a - b);
				break;
			case '*':
				result.push(a * b);
				break;
			case '/':
				if (fabs(b) < 0.000001)
					throw exception("Divide zero.");
				result.push(a / b);
				break;
			default:
				break;
			}
		}
	}
	if (result.size() > 1)
		throw exception("Wrong format.");
	double ret = result.top();
	ret = round(ret * 10) / 10.0;
	return ret;
}

std::string Calculator::perform(std::string input)
{
	try
	{
		auto mid = __divide(input);
		__check(mid);
		auto back = __midToBack(mid);
		auto result = format("{:.1f}", __calculate(back));
		if (result.ends_with('0'))
			result.pop_back(), result.pop_back();
		return result;
	}
	catch (const std::exception& e)
	{
		return e.what();
	}
	return std::string();
}
