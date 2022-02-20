#pragma once
#include <iostream>
bool WelcomePage();

int getOperationCode() {
	std::string codeString;
	int codeInt;
	std::cout << "输入操作：";
	std::cin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::cout << "不正确的输入\n";
		codeInt = getOperationCode();
	}
	return codeInt;
}

void AdministatorPage();