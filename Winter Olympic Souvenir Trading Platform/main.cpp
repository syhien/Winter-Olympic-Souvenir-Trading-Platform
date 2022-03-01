#include <iostream>
#include <string>
#include "main.h"
#include "Administator.h"
#include "Database.h"
#include "terminal.h"
#include <codecvt>
#include <locale>
#include "Calculator.h"
#include <format>
#include <Windows.h>
using namespace std;

Database database;

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "chs");
	wcout.imbue(locale("chs"));

	Administator administator(&database);

	while (WelcomePage(administator));

	cout << "退出程序\n";
	return 0;
}

bool WelcomePage(Administator& administator) {
	int operationCode;
	wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.用户登录", L"2.用户注册", L"3.退出程序", L"4.管理员登录");
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
		logIn();
		break;
	case 2:
		//
		break;
	case 3:
		cout << "退出欢迎页\n";
		return false;
	case 4:
		AdministatorPage(administator);
		break;
	default:
		break;
	}
	return true;
}

void AdministatorPage(Administator& administator)
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

static string wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void logIn()
{
	bool checkName = false;
	bool checkPassword = false;
	vector<pair<string, wstring> > userInfo;
	while (!checkName or !checkPassword)
	{
		wstring name, password;
		cout << "请输入用户名：";
		wcin >> name;
		cout << "请输入密码：";
		wcin >> password;
		for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(name))) {
			for (auto& j : i) {
				if (j.first == "name" and j.second == name)
					checkName = true;
				if (j.first == "password" and j.second == password)
					checkPassword = true;
			}
			if (checkName and checkPassword)
				userInfo = i;
		}
		if (!checkName or !checkPassword) {
			cout << "账号或密码错误\n是否再次尝试？\n输入1重试，输入其他任意数字放弃重试\n";
			if (getOperationCode() != 1)
				return;
		}
	}
	cout << "登录成功\n";
}

void signUp()
{
}
