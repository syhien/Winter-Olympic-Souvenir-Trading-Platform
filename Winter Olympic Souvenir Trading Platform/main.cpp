#include <iostream>
#include <string>
#include "main.h"
using namespace std;

int main(int argc, char* args[]) {
	while (WelcomePage());

	cout << "退出程序\n";
	return 0;
}

bool WelcomePage() {
	int operationCode;
	cout << "1.用户登录  2.用户注册  3.退出程序  4.管理员登录\n";
	while (true)
	{
		operationCode = getOperationCode();
		if (operationCode == 1 or operationCode == 2 or operationCode == 3 or operationCode == 4)
			break;
		else
			cout << "无此操作\n";
	}
	switch (operationCode)
	{
	case 1:
		//
	case 2:
		//
	case 3:
		cout << "退出欢迎页\n";
		return false;
	case 4:
		AdministatorPage();
	default:
		break;
	}
	return true;
}

void AdministatorPage()
{
	cout << "todo\n";
}