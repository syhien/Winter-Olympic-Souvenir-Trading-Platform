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
#include <regex>
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
	string id;
	for (auto& i : userInfo | views::filter([&](const pair<string, wstring>& j) {return j.first == "userID"; }))
		id = wstring2string(i.second);
	while (keepHere)
	{
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.我是买家", L"2.我是卖家", L"3.管理个人信息", L"4.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			BuyerPage(id);
			break;
		case 2:
			SellerPage(id);
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
	wstring oldValue;
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
					oldValue = i.second;
					i.second = format(L"{:.1f}", stod(oldValue) + stod(newValue));
				}
				database.perform("UPDATE user SET wallet = " + wstring2string(format(L"{:.1f}", stod(oldValue) + stod(newValue))) + " WHERE userID = " + wstring2string(userID));
				database.perform("INSERT INTO recharge VALUES (" + wstring2string(userID) + "," + wstring2string(newValue) + "," + getCurrentTime() + ")");
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

void SellerPage(std::string id)
{
	bool keepHere = true;
	wstring tmp;
	string command;
	auto allCommodity = database.perform("SELECT * FROM commodity");
	vector<string> newCommodity;
	while (keepHere) {
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.发布商品", L"2.查看发布商品", L"3.修改商品信息", L"4.下架商品", L"5.查看历史订单", L"6.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			command = "INSERT INTO commodity VALUES (";
			for (int i = 0; i < 999; i++) {
				bool existed = false;
				for (auto& j : allCommodity)
					for (auto& k : j | views::filter([i](const pair<string, wstring> l) {return l.first == "itemID"; }))
						existed = existed and k.second != format(L"M{:3}", i);
				if (!existed) {
					newCommodity.push_back(format("M{:3}", i));
					break;
				}
			}

			cout << "请输入商品名称：";
			wcin >> tmp;
			if (tmp.length() > 20)
			{
				cout << "商品名称过长，请重试" << endl;
				break;
			}
			newCommodity.push_back(wstring2string(tmp));

			cout << "请输入价格（至多精确至小数点后1位）：";
			wcin >> tmp;
			try
			{
				stod(tmp);
			}
			catch (const std::exception&)
			{
				cout << "不正确的输入，请重试" << endl;
				break;
			}
			newCommodity.push_back(format("{:.1f}", stod(tmp)));

			cout << "请输入数量：";
			wcin >> tmp;
			try
			{
				stoi(tmp);
				if (stoi(tmp) <= 0)
					throw "";
			}
			catch (const std::exception&)
			{
				cout << "不正确的输入，请重试" << endl;
				break;
			}
			newCommodity.push_back(format("{}", stoi(tmp)));

			cout << "请输入200字内的商品描述：";
			wcin >> tmp;
			tmp = regex_replace(tmp, wregex(L"\."), L"，");
			if (tmp.length() > 200) {
				cout << "不正确的输入，请重试" << endl;
				break;
			}
			newCommodity.push_back(wstring2string(tmp));

			newCommodity.push_back(id);
			newCommodity.push_back(getCurrentTime());
			newCommodity.push_back(wstring2string(L"销售中"));

			cout << "请确认待添加商品的信息：" << endl << endl;
			cout << "商品名称：" << newCommodity[1] << endl;
			cout << "商品价格：" << newCommodity[2] << endl;
			cout << "商品数量：" << newCommodity[3] << endl;
			wcout << L"商品描述：" << string2wstring(newCommodity[4]) << endl << endl;

			cout << "是否添加商品？输入1以添加，输入其他数字以放弃添加" << endl;
			if (getOperationCode() == 1)
			{
				try
				{
					for (auto& i : newCommodity)
						command += i + ",";
					command.pop_back();
					command += ")";
					database.perform(command);
					cout << "添加成功" << endl;
				}
				catch (const std::exception&)
				{
					cout << "操作未生效" << endl;
				}
			}
			else
			{
				cout << "放弃添加" << endl;
			}

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

void BuyerPage(std::string id)
{
}

void SignUp()
{
}
