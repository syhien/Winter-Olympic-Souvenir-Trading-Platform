#include "pch.h"
#include "../Winter Olympic Souvenir Trading Platform/Calculator.h"
using namespace std;


TEST(CalculatorTest, TestOperandWrongFormat) {
	auto calculator = Calculator();
	EXPECT_EQ(calculator.perform(".52358"), "Wrong number format.");
	EXPECT_EQ(calculator.perform("35.5322.1"), "Wrong number format.");
	EXPECT_EQ(calculator.perform("35."), "Wrong number format.");
	EXPECT_EQ(calculator.perform("1..1+1"), "Wrong number format.");
}

TEST(CalculatorTest, TestUnknownChar) {
	auto calculator = Calculator();
	EXPECT_EQ(calculator.perform("&"), "Unknown char.");
	EXPECT_EQ(calculator.perform("gwe"), "Unknown char.");
	EXPECT_EQ(calculator.perform(",,//?"), "Unknown char.");
	EXPECT_EQ(calculator.perform("y34f3=\\"), "Unknown char.");
	EXPECT_EQ(calculator.perform("```"), "Unknown char.");
}

TEST(CalculatorTest, TestContinousOperand) {
	auto calculator = Calculator();
	set<string> possibleErrors = { "Continuous operands." ,"Missing operator." ,"Wrong format." };
	EXPECT_TRUE(possibleErrors.count(calculator.perform("12(-3)")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("12 3")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("12-(359+2/3) 3")) == 1) << calculator.perform("12-(359+2/3) 3");
}

TEST(CalculatorTest, TestMissingOperand) {
	auto calculator = Calculator();
	set<string> possibleErrors = { "Missing operand." ,"Wrong format." };
	EXPECT_TRUE(possibleErrors.count(calculator.perform("12 ++3")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("--3")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("()")) == 1);
	EXPECT_EQ(calculator.perform("-1"), "-1");
}

TEST(CalculatorTest, TestPairedParenthesis) {
	auto calculator = Calculator();
	set<string> possibleErrors = { "Missing paired parentheses." ,"Wrong format." };
	EXPECT_TRUE(possibleErrors.count(calculator.perform("-12) +3")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("((1+1)* (2-1)")) == 1) << calculator.perform("((1+1)* (2-1)");
	EXPECT_EQ(calculator.perform("(-1)"), "-1");
}

TEST(CalculatorTest, TestDivideZero) {
	auto calculator = Calculator();
	set<string> possibleErrors = { "Divide zero." };
	EXPECT_TRUE(possibleErrors.count(calculator.perform("1/(2* 4-8)")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("5745/0")) == 1);
	EXPECT_TRUE(possibleErrors.count(calculator.perform("1/0.0000001")) == 1);
}

TEST(CalculatorTest, TestResult) {
	auto calculator = Calculator();
	EXPECT_EQ(calculator.perform("((4* 3+2)/(6.4-2.4)-10)* (6.4-7.2)+(-2)* 4"), "-2.8");
	EXPECT_EQ(calculator.perform(" 2 * 3 / 4"), "1.5");
	EXPECT_EQ(calculator.perform("-2.05"), "-2.1");
	EXPECT_EQ(calculator.perform("2.04"), "2");
	EXPECT_EQ(calculator.perform("1.1+0.9"), "2");
	EXPECT_EQ(calculator.perform("318-30-318*0.1-318*60/800"), "232.4");
	EXPECT_EQ(calculator.perform("1 * (10+(112.2)) + 30*(10.0)"), "422.2");
}