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
#include <ranges>
#include <Windows.h>
#include <ctype.h>
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
		LogIn();
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

void LogIn()
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
				if (j.first == "state" and j.second == L"封禁") {
					cout << "用户已被封禁" << endl;
					return;
				}
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
	bool keepHere = true;
	while (keepHere)
	{
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.我是买家", L"2.我是卖家", L"3.管理个人信息", L"4.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			BuyerPage();
			break;
		case 2:
			SellerPage();
			break;
		case 3:
			InfoManagePage(userInfo);
			break;
		case 4:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

void InfoManagePage(std::vector<std::pair<std::string, std::wstring>>& userInfo)
{
	bool keepHere = true;
	wstring newValue;
	bool editSuccess = false;
	wstring userID;
	for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "userID"; }))
		userID = i.second;
	while (keepHere)
	{
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.查看信息", L"2.修改信息", L"3.充值", L"4.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"用户名", L"联系方式", L"地址", L"钱包余额");
			for (auto& i : userInfo) {
				if (i.first == "name") {
					wcout << format(L"|{:^37}", i.second);
				}
				else if (i.first == "contact") {
					wcout << format(L"|{:^37}", i.second);
				}
				else if (i.first == "address") {
					wcout << format(L"|{:^37}", i.second);
				}
				else if (i.first == "wallet") {
					wcout << format(L"|{:^37}", i.second);
				}
			}
			wcout << "|\n";
			break;
		case 2:
			cout << "请选择想要本次修改的信息" << endl;
			wcout << format(L"\n|{:^30}|{:^30}|{:^30}|{:^30}|{:^30}|\n", L"1.用户名", L"2.密码", L"3.联系方式", L"4.地址", L"5.放弃本次修改");
			switch (getOperationCode())
			{
			case 1:
				editSuccess = true;
				cout << "请输入新用户名：";
				wcin >> newValue;
				if (newValue.size() > 10) {
					cout << "用户名过长" << endl;
					break;
				}
				for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(newValue)))
					for (auto& j : i)
						if (j.first == "name" and j.second == newValue) {
							editSuccess = false;
							cout << "该用户名已被占用" << endl;
							break;
						}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "name"; }))
							i.second = newValue;
						database.perform("UPDATE user SET name = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
						cout << "修改用户名成功" << endl;
					}
					catch (const std::exception&)
					{
						cout << "操作未生效" << endl;
					}
				}
				else {
					cout << "操作未生效" << endl;
				}
				break;
			case 2:
				editSuccess = true;
				cout << "请输入新密码：";
				wcin >> newValue;
				if (newValue.size() > 20) {
					cout << "密码过长" << endl;
					break;
				}
				for (auto& i : newValue) {
					if (!isdigit(i) and !islower(i))
					{
						cout << "密码仅允许由小写字母和数字组成" << endl;
						editSuccess = false;
						break;
					}
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "password"; }))
							i.second = newValue;
						database.perform("UPDATE user SET password = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
						cout << "修改密码成功" << endl;
					}
					catch (const std::exception&)
					{
						cout << "操作未生效" << endl;
					}
				}
				else {
					cout << "操作未生效" << endl;
				}
				break;
			case 3:
				editSuccess = true;
				cout << "请输入新联系方式：";
				wcin >> newValue;
				if (newValue.size() > 20) {
					cout << "联系方式过长" << endl;
					break;
				}
				for (auto& i : newValue | views::filter([](const wchar_t j) {return !isdigit(j); })) {
					cout << "联系方式仅允许由数字组成" << endl;
					editSuccess = false;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "contact"; }))
							i.second = newValue;
						database.perform("UPDATE user SET contact = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
						cout << "修改联系方式成功" << endl;
					}
					catch (const std::exception&)
					{
						cout << "操作未生效" << endl;
					}
				}
				else {
					cout << "操作未生效" << endl;
				}
				break;
			case 4:
				editSuccess = true;
				cout << "请输入新地址：";
				wcin >> newValue;
				if (newValue.size() > 40) {
					cout << "地址过长" << endl;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "address"; }))
							i.second = newValue;
						database.perform("UPDATE user SET address = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
						cout << "修改地址成功" << endl;
					}
					catch (const std::exception&)
					{
						cout << "操作未生效" << endl;
					}
				}
				else {
					cout << "操作未生效" << endl;
				}
				break;
			case 5:
				cout << "放弃修改" << endl;
				break;
			default:
				break;
			}
			break;
		case 3:
			cout << "请输入充值金额：";
			wcin >> newValue;
			try
			{
				for (auto &i:newValue)
					if (!isdigit(i) and i != '.') {
						throw exception();
					}
				if (count(newValue.begin(), newValue.end(), '.') > 1)
					throw exception();
				else if (count(newValue.begin(), newValue.end(), '.') == 1) {
					if (newValue[newValue.size() - 2] != '.' or newValue[0] == '.')
						throw exception();
				}
				for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring>& j) {return j.first == "wallet"; })) {
					i.second = format(L"{:.1f}", stod(i.second) + stod(newValue));
					newValue = i.second;
				}
				database.perform("UPDATE user SET wallet = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
			}
			catch (const std::exception&)
			{
				cout << "金额仅支持最多含1位小数的非负数值" << endl;
			}
			break;
		case 4:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

void SellerPage()
{
}

void BuyerPage()
{
}

void SignUp()
{
}
