#include <iostream>
#include <string>
#include "main.h"
#include "Administator.h"
#include "Database.h"
#include "terminal.h"
#include <codecvt>
#include <locale>
using namespace std;

Database database;
Administator administator(database);

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "chs");
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wcout.imbue(locale("chs"));
	auto result = database.perform("SELECT * FROM user");
	for (auto& i : result) {
		for (auto& j : i) {
			std::wcout << L"key=" + converter.from_bytes(j.first) + L", value=" + converter.from_bytes(j.second) << std::endl;
		}
	}
	while (WelcomePage());

	cout << "退出程序\n";
	return 0;
}

bool WelcomePage() {
	int operationCode;
	User user;
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
		break;
	case 2:
		//
		break;
	case 3:
		cout << "退出欢迎页\n";
		return false;
	case 4:
		AdministatorPage();
		break;
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