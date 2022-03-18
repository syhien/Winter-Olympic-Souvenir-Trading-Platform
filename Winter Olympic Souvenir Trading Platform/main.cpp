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
#include <queue>
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
		SignUp();
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
		for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(name), "admin", "123456")) {
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
					try
					{
						if (stod(calculateWallet(wstring2string(userID))) < 0)
							throw "";
						database.perform("UPDATE user SET wallet = " + calculateWallet(wstring2string(userID)) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
						wcout << format(L"|{:^37}", string2wstring(calculateWallet(wstring2string(userID))));
					}
					catch (const std::exception&)
					{
						cout << endl << "余额异常" << endl;
					}
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
				for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(newValue), wstring2string(userID), "user"))
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
						database.perform("UPDATE user SET name = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
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
						database.perform("UPDATE user SET password = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
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
						database.perform("UPDATE user SET contact = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
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
						database.perform("UPDATE user SET address = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
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
				database.perform("UPDATE user SET wallet = " + wstring2string(format(L"{:.1f}", stod(oldValue) + stod(newValue))) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
				database.perform("INSERT INTO recharge VALUES (" + wstring2string(userID) + "," + wstring2string(newValue) + "," + getCurrentTime() + ")", wstring2string(userID), "user");
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
	auto allCommodity = std::vector<std::vector<std::pair<std::string, std::wstring>>>();
	auto allOrder = std::vector<std::vector<std::pair<std::string, std::wstring>>>();
	vector<string> newCommodity;
	string itemID;
	string updateKey, updateValue;
	while (keepHere) {
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.发布商品", L"2.查看发布商品", L"3.修改商品信息", L"4.下架商品", L"5.查看历史订单", L"6.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			command = "INSERT INTO commodity VALUES (";
			newCommodity.clear();
			for (int i = 1; i < 999; i++) {
				bool existed = false;
				for (auto& j : allCommodity)
					for (auto& k : j | views::filter([i](const pair<string, wstring> l) {return l.first == "itemID"; }))
						existed = existed or k.second == format(L"M{:0>3}", i);
				if (!existed) {
					newCommodity.push_back(format("M{:0>3}", i));
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
			tmp = regex_replace(tmp, wregex(L","), L"，");
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
					database.perform(command, id, "seller");
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
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allCommodity.end());
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 3:
			updateKey = "dont";
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allCommodity.end());
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "请输入想要修改信息的商品的商品ID：";
			cin >> itemID;
			cout << "请选择想要本次修改的信息" << endl;
			wcout << format(L"\n|{:^38}|{:^38}|{:^38}|{:^38}|\n", L"1.名称", L"2.价格", L"3.数量", L"4.描述");
			switch (getOperationCode())
			{
			case 1:
				cout << "请输入新名称：";
				wcin >> tmp;
				if (tmp.length() > 20)
				{
					cout << "商品名称过长，请重试" << endl;
					break;
				}
				updateKey = "name";
				updateValue = wstring2string(tmp);
				break;
			case 2:
				cout << "请输入新价格：";
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
				updateKey = "price";
				updateValue = format("{:.1f}", stod(tmp));
				break;
			case 3:
				cout << "请输入新数量：";
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
				updateKey = "count";
				updateValue = format("{}", stoi(tmp));
				break;
			case 4:
				cout << "请输入新描述：";
				wcin >> tmp;
				tmp = regex_replace(tmp, wregex(L","), L"，");
				if (tmp.length() > 200) {
					cout << "不正确的输入，请重试" << endl;
					break;
				}
				updateKey = "description";
				updateValue = wstring2string(tmp);
				break;
			default:
				break;
			}
			if (updateKey == "dont")
				break;
			try
			{
				command = format("UPDATE commodity SET {} = {} WHERE itemID = {}", updateKey, updateValue, itemID);
				database.perform(command, id, "seller");
				cout << "添加成功" << endl;
			}
			catch (const std::exception&)
			{
				cout << "操作未生效" << endl;
			}
			break;
		case 4:
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allCommodity.end());
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "请输入想要下架的商品的商品ID：";
			cin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "state") return j.second != L"销售中"; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != string2wstring(itemID); } return true; }), allCommodity.end());
			if (allCommodity.empty()) {
				cout << "无可下架商品的商品ID为" << itemID << endl;
				break;
			}
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "请确认是否下架商品，输入1以下架商品，输入其他数字取消下架\n";
			if (getOperationCode() == 1) {
				try
				{
					database.perform("UPDATE commodity SET state = 已下架 WHERE itemID = " + itemID, id, "seller");
					cout << "下架成功" << endl;
				}
				catch (const std::exception&)
				{
					cout << "操作未生效" << endl;
				}
			}
			else
				cout << "放弃下架商品" << endl;
			break;
		case 5:
			allOrder = database.perform("SELECT * FROM order WHERE sellerID CONTAINS " + id, id, "seller");
			allOrder.erase(remove_if(allOrder.begin(), allOrder.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allOrder.end());
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "订单ID", "商品ID", "交易单价", "数量", "交易时间", "卖家ID", "买家ID");
			for (auto& line : allOrder)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
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
	bool keepHere = true;
	auto allCommodity = std::vector<std::vector<std::pair<std::string, std::wstring>>>();
	auto allOrder = std::vector<std::vector<std::pair<std::string, std::wstring>>>();
	wstring itemID;
	int count;
	int availableCount;
	double price;
	string newOrderID;
	string sellerID;
	wstring keyword;
	vector<wstring> titles;
	while (keepHere)
	{
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.查看商品列表", L"2.购买商品", L"3.搜索商品", L"4.查看历史订单", L"5.查看商品详细信息", L"6.返回上层菜单");
		switch (getOperationCode())
		{
		case 1:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"销售中"), id, "buyer");
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"商品ID", L"商品名称", L"价格", L"数量", L"卖家ID", L"上架时间");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						wcout << format(L"|{:^25}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"销售中"), id, "buyer");
			cout << "请输入商品ID：";
			wcin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) == id; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != itemID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "商品不存在或不可购买" << endl;
				break;
			}
			cout << "请输入购买数量：";
			cin >> count;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [count](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "count") return stoi(j.second) < count; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "商品数量不足，无法购买" << endl;
				break;
			}
			for (auto& i : allCommodity.front())
				if (i.first == "price")
					price = stod(i.second);
				else if (i.first == "count")
					availableCount = stoi(i.second);
				else if (i.first == "sellerID")
					sellerID = wstring2string(i.second);
			if (stod(calculateWallet(id)) < price * count) {
				cout << "余额不足，无法购买" << endl;
				break;
			}
			try
			{
				if (count == availableCount)
					database.perform("UPDATE commodity SET count = 0, state = " + wstring2string(L"已下架") + " WHERE itemID = " + wstring2string(itemID), id, "buyer");
				else
					database.perform("UPDATE commodity SET count = " + to_string(availableCount - count) + " WHERE itemID = " + wstring2string(itemID), id, "buyer");
				allOrder = database.perform("SELECT * FROM order", id, "buyer");
				for (int i = 1; i < 999; i++) {
					bool existed = false;
					for (auto& j : allOrder)
						for (auto& k : j | views::filter([i](const pair<string, wstring> l) {return l.first == "orderID"; }))
							existed = existed or k.second == format(L"T{:0>3}", i);
					if (!existed) {
						newOrderID = format("M{:0>3}", i);
						break;
					}
				}
				database.perform("INSERT INTO order VALUES " + format("({},{},{:.1f},{},{},{},{})", newOrderID, wstring2string(itemID), price, count, getCurrentTime(), sellerID, id), id, "buyer");
				database.perform("UPDATE user SET wallet = " + calculateWallet(id) + " WHERE userID = " + id, id, "buyer");
			}
			catch (const std::exception&)
			{
				cout << "操作未生效" << endl;
			}
			break;
		case 3:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"销售中"), id, "buyer");
			cout << "请输入关键字：";
			wcin >> keyword;
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"商品ID", L"商品名称", L"价格", L"数量", L"卖家ID", L"上架时间");
			for (auto& line : allCommodity | views::filter([keyword](const vector<pair<string, wstring> >& i) {for (auto& j : i) { if (j.second.find(keyword) != wstring::npos) return true; } return false; })) {
				wcout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						wcout << format(L"|{:^25}", i.second);
				wcout << "|\n";
			}
			break;
		case 4:
			allOrder = database.perform("SELECT * FROM order WHERE buyerID CONTAINS " + id, id, "buyer");
			allOrder.erase(remove_if(allOrder.begin(), allOrder.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "buyerID") return wstring2string(j.second) != id; } return true; }), allOrder.end());
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "订单ID", "商品ID", "交易单价", "数量", "交易时间", "卖家ID", "买家ID");
			for (auto& line : allOrder)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 5:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"销售中"), id, "buyer");
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"商品ID", L"商品名称", L"价格", L"数量", L"卖家ID", L"上架时间");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						wcout << format(L"|{:^25}", i.second);
				wcout << "|\n";
			}
			cout << "请输入想要查看的商品ID：";
			wcin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != itemID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "商品不存在或不可购买" << endl;
				break;
			}
			titles = { L"商品ID", L"商品名称", L"价格", L"数量", L"描述", L"卖家ID", L"上架时间", L"销售状态" };
			cout << endl;
			for (auto& i : allCommodity.front())
				wcout << titles.front() << L"：" << i.second << endl, titles.erase(titles.begin());
			cout << endl;
			break;
		case 6:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

string calculateWallet(std::string userID)
{
	Calculator calculator;
	auto allRecharge = database.perform("SELECT * FROM recharge WHERE userID CONTAINS " + userID, userID, "user");
	allRecharge.erase(remove_if(allRecharge.begin(), allRecharge.end(), [userID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "userID") return wstring2string(j.second) != userID; } return true; }), allRecharge.end());
	auto allBuyOrder = database.perform("SELECT * FROM order WHERE buyerID CONTAINS " + userID, userID, "user");
	allBuyOrder.erase(remove_if(allBuyOrder.begin(), allBuyOrder.end(), [userID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "buyerID") return wstring2string(j.second) != userID; } return true; }), allBuyOrder.end());
	auto allSellOrder = database.perform("SELECT * FROM order WHERE sellerID CONTAINS " + userID, userID, "user");
	allSellOrder.erase(remove_if(allSellOrder.begin(), allSellOrder.end(), [userID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != userID; } return true; }), allSellOrder.end());
	string command = "";
	map<wstring, int> numbers;
	for (auto& i : allRecharge)
		for (auto& j : i)
			if (j.first == "money") {
				if (numbers.contains(j.second))
					numbers[j.second]++;
				else
					numbers[j.second] = 1;
			}
	for (auto& i : allBuyOrder) {
		int count;
		wstring price;
		for (auto& j : i)
			if (j.first == "count")
				count = stoi(j.second);
			else if (j.first == "price")
				price = L"-" + j.second;
		if (numbers.contains(price))
			numbers[price] += count;
		else
			numbers[price] = count;
	}
	for (auto& i : allSellOrder) {
		int count;
		wstring price;
		for (auto& j : i)
			if (j.first == "count")
				count = stoi(j.second);
			else if (j.first == "price")
				price = j.second;
		if (numbers.contains(price))
			numbers[price] += count;
		else
			numbers[price] = count;
	}
	map<int, vector<string>> countNumbers;
	for (auto& i : numbers)
		countNumbers[i.second].push_back(wstring2string(i.first));
	for (auto& i : countNumbers) {
		if (command.length() == 0)
			command += format("{}", i.first) + " * ";
		else
			command += " + " + format("{}", i.first) + "*";
		command += "(";

		string subCommand = "";
		for (auto& j : i.second)
			if (subCommand.length() == 0)
				subCommand += format("{}", j);
			else
				subCommand += "+" + format("({})", j);
		command += subCommand;

		command += ")";
	}
	if (command.length() == 0)
		return "0.0";
	try
	{
		return calculator.perform(command);
	}
	catch (const std::exception&)
	{
		return "";
	}
}

void SignUp()
{
	auto allUser = database.perform("SELECT * FROM user", "admin", "123456");
	vector<string> newUser;
	for (int i = 1; i < 999; i++) {
		bool existed = false;
		for (auto& j : allUser)
			for (auto& k : j | views::filter([i](const pair<string, wstring>& l) {return l.first == "userID"; }))
				existed = existed or k.second == format(L"U{:0>3}", i);
		if (!existed) {
			newUser.push_back(format("U{:0>3}", i));
			break;
		}
	}

	wstring tmp;
	cout << "请输入用户名：";
	wcin >> tmp;
	if (tmp.length() > 10) {
		cout << "用户名过长" << endl;
		return;
	}
	auto nameFound = false;
	for (auto& i : allUser)
		for (auto& j : i)
			if (j.first == "name")
				nameFound = nameFound or j.second == tmp;
	if (nameFound) {
		cout << "该用户名已被注册" << endl;
		return;
	}
	newUser.push_back(wstring2string(tmp));

	cout << "请输入密码：";
	wcin >> tmp;
	if (tmp.size() > 20) {
		cout << "密码过长" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i) and !islower(i))
		{
			cout << "密码仅允许由小写字母和数字组成" << endl;
			return;
		}
	}
	newUser.push_back(wstring2string(tmp));
	
	cout << "请输入联系方式：";
	wcin >> tmp;
	if (tmp.size() > 20) {
		cout << "联系方式过长" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i))
		{
			cout << "联系方式仅允许由数字组成" << endl;
			return;
		}
	}
	newUser.push_back(wstring2string(tmp));

	cout << "请输入联络地址：";
	wcin >> tmp;
	if (tmp.size() > 40) {
		cout << "联络地址过长" << endl;
		return;
	}
	newUser.push_back(wstring2string(tmp));

	newUser.push_back("0.0");

	newUser.push_back(wstring2string(L"正常"));

	cout << "请确认注册信息：" << endl << endl;
	wcout << L"用户名：" << string2wstring(newUser[1]) << endl;
	wcout << L"密码：" << string2wstring(newUser[2]) << endl;
	wcout << L"联系方式：" << string2wstring(newUser[3]) << endl;
	wcout << L"联络地址：" << string2wstring(newUser[4]) << endl;
	cout << endl;

	cout << "输入1以确认注册，输入其他数字取消注册" << endl;
	if (getOperationCode() == 1) {
		try
		{
			string command = "INSERT INTO user VALUES (";
			for (auto& i : newUser)
				command += i + ",";
			command.pop_back();
			command += ")";
			database.perform(command, "admin", "123456");
			cout << "注册成功，返回上层页面" << endl;
		}
		catch (const std::exception&)
		{
			cout << "操作未生效" << endl;
		}
	}
	else {
		cout << "放弃注册" << endl;
	}
}
