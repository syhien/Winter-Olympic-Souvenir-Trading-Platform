#include "Administator.h"
#include "terminal.h"
#include <format>
#include <ranges>
#include <Windows.h>
using namespace std;

Administator::Administator(Database* database)
{
	__database = database;
}

bool Administator::login(std::string name, std::string password)
{
	return name == __name and password == __password;
}

void Administator::HomePage()
{
	bool login = true;
	while (login) {
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.商品操作", L"2.订单操作", L"3.用户操作", L"4.登出");
		auto operationCode = getOperationCode();
		switch (operationCode)
		{
		case 1:
			__handleCommodity();
			break;
		case 2:
			__handleOrder();
			break;
		case 3:
			__handleUser();
			break;
		case 4:
			cout << "管理员登出\n";
			login = false;
			break;
		default:
			cout << "无此操作\n";
			break;
		}
	}
}

string wstring2string(wstring wstr)
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

void Administator::__handleCommodity()
{
	bool keepHere = true;
	auto allCommodity = __database->perform("SELECT * FROM commodity");
	auto selectedCommodity = allCommodity;
	int tmpInt;
	wstring keyword;
	while (keepHere) {
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.查看所有商品", L"2.检索商品", L"3.下架商品", L"4.返回上层菜单");
		auto operationCode = getOperationCode();
		allCommodity = __database->perform("SELECT * FROM commodity");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << "请输入关键词：";
			wcin >> keyword;
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto line : allCommodity | views::filter([keyword](const vector<pair<string, wstring> > i) {for (auto j : i) { if (j.second.find(keyword) != wstring::npos) return true; } return false; })) {
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 3:
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "请输入待下架的商品的商品ID：";
			wcin >> keyword;
			try
			{
				selectedCommodity = __database->perform("SELECT * FROM commodity WHERE itemID CONTAINS " + wstring2string(keyword));
				cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "商品ID", "名称", "价格", "数量", "描述", "卖家ID", "上架时间", "商品状态");
				for (auto& line : selectedCommodity)
				{
					wcout << endl;
					for (auto& i : line)
						wcout << format(L"|{:^19}", i.second);
					wcout << "|\n";
				}
				cout << "请确认是否下架商品，输入1以下架商品，输入其他数字取消下架\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "已取消下架" << endl;
					break;
				}
				__database->perform("UPDATE commodity SET state = 已下架 WHERE itemID = " + wstring2string(keyword));
				cout << "下架成功\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "操作未生效\n";
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

void Administator::__handleOrder()
{
	bool keepHere = true;
	auto allOrder = __database->perform("SELECT * FROM order");
	wstring keyword;
	while (keepHere) {
		wcout << format(L"\n|{:^50}|{:^50}|{:^50}|\n", L"1.查看所有订单", L"2.检索订单", L"3.返回上层菜单");
		auto operationCode = getOperationCode();
		allOrder = __database->perform("SELECT * FROM order");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "订单ID", "商品ID", "交易单价", "数量", "交易时间", "卖家ID", "买家ID");
			for (auto& line : allOrder)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << "请输入关键词：";
			wcin >> keyword;
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "订单ID", "商品ID", "交易单价", "数量", "交易时间", "卖家ID", "买家ID");
			for (auto line : allOrder | views::filter([keyword](const vector<pair<string, wstring> > i) {for (auto j : i) { if (j.second.find(keyword) != wstring::npos) return true; } return false; })) {
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 3:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

void Administator::__handleUser()
{
	bool keepHere = true;
	auto allUser = __database->perform("SELECT * FROM user");
	auto selectedUser = allUser;
	wstring keyword;
	int tmpInt;
	while (keepHere) {
		wcout << format(L"\n|{:^50}|{:^50}|{:^50}|\n", L"1.查看所有用户", L"2.封禁用户", L"3.返回上层菜单");
		auto operationCode = getOperationCode();
		allUser = __database->perform("SELECT * FROM user");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "用户ID", "用户名", "密码", "联系方式", "地址", "钱包余额", "用户状态");
			for (auto& line : allUser)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "用户ID", "用户名", "密码", "联系方式", "地址", "钱包余额", "用户状态");
			for (auto& line : allUser)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			cout << "请输入待封禁的用户的用户ID：";
			wcin >> keyword;
			try
			{
				selectedUser = __database->perform("SELECT * FROM user WHERE userID CONTAINS " + wstring2string(keyword));
				cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "用户ID", "用户名", "密码", "联系方式", "地址", "钱包余额", "用户状态");
				for (auto& line : selectedUser)
				{
					wcout << endl;
					for (auto& i : line)
						wcout << format(L"|{:^21}", i.second);
					wcout << "|\n";
				}
				cout << "请确认是否封禁用户，输入1以封禁用户，输入其他数字取消封禁\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "已取消封禁" << endl;
					break;
				}
				__database->perform("UPDATE user SET state = 封禁 WHERE userID = " + wstring2string(keyword));
				cout << "封禁成功\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "操作未生效\n";
			}

			break;
		case 3:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}
