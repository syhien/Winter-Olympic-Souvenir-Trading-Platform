#include <iostream>
#include <string>
#include "main.h"
#include "Administator.h"
#include "Database.h"
using namespace std;

Database database;
Administator administator(database);

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
	string name, password;
	cout << "输入管理员账户名：";
	cin >> name;
	cout << "输入管理员密码：";
	cin >> password;
	while (administator.login(name, password) == false)
	{
		cout << "管理员账号或密码错误\n是否再次尝试？\n输入1重试，输入其他任意数字放弃重试\n";
		if (getOperationCode() != 1)
			return;
		cout << "输入管理员账户名：";
		cin >> name;
		cout << "输入管理员密码：";
		cin >> password;
	}
	cout << "登录成功！\n";
	administator.HomePage();
}