#include "terminal.h"
#include <string>
#include <format>
#include <vector>
using namespace std;

int getOperationCode()
{
	std::string codeString;
	int codeInt;
	std::cout << "���������";
	std::cin >> codeString;
	try
	{
		codeInt = std::stoi(codeString);
	}
	catch (const std::exception&)
	{
		std::cout << "����ȷ������\n";
		codeInt = getOperationCode();
	}
	return codeInt;
}