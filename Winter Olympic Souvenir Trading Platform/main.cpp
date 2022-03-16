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

	cout << "�˳�����\n";
	return 0;
}

bool WelcomePage(Administator& administator) {
	int operationCode;
	wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.�û���¼", L"2.�û�ע��", L"3.�˳�����", L"4.����Ա��¼");
	while (true)
	{
		operationCode = getOperationCode();
		if (operationCode == 1 or operationCode == 2 or operationCode == 3 or operationCode == 4)
			break;
		else
			cout << "�޴˲���\n";
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
		cout << "�˳���ӭҳ\n";
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
	cout << "�������Ա�˻�����";
	cin >> name;
	cout << "�������Ա���룺";
	cin >> password;
	while (administator.login(name, password) == false)
	{
		cout << "����Ա�˺Ż��������\n�Ƿ��ٴγ��ԣ�\n����1���ԣ����������������ַ�������\n";
		if (getOperationCode() != 1)
			return;
		cout << "�������Ա�˻�����";
		cin >> name;
		cout << "�������Ա���룺";
		cin >> password;
	}
	cout << "��¼�ɹ���\n";
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
		cout << "�������û�����";
		wcin >> name;
		cout << "���������룺";
		wcin >> password;
		for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(name), "admin", "123456")) {
			for (auto& j : i) {
				if (j.first == "name" and j.second == name)
					checkName = true;
				if (j.first == "password" and j.second == password)
					checkPassword = true;
				if (j.first == "state" and j.second == L"���") {
					cout << "�û��ѱ����" << endl;
					return;
				}
			}
			if (checkName and checkPassword)
				userInfo = i;
		}
		if (!checkName or !checkPassword) {
			cout << "�˺Ż��������\n�Ƿ��ٴγ��ԣ�\n����1���ԣ����������������ַ�������\n";
			if (getOperationCode() != 1)
				return;
		}
	}
	cout << "��¼�ɹ�\n";
	bool keepHere = true;
	string id;
	for (auto& i : userInfo | views::filter([&](const pair<string, wstring>& j) {return j.first == "userID"; }))
		id = wstring2string(i.second);
	while (keepHere)
	{
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.�������", L"2.��������", L"3.���������Ϣ", L"4.�����ϲ�˵�");
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
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.�鿴��Ϣ", L"2.�޸���Ϣ", L"3.��ֵ", L"4.�����ϲ�˵�");
		switch (getOperationCode())
		{
		case 1:
			wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"�û���", L"��ϵ��ʽ", L"��ַ", L"Ǯ�����");
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
						cout << endl << "����쳣" << endl;
					}
				}
			}
			wcout << "|\n";
			break;
		case 2:
			cout << "��ѡ����Ҫ�����޸ĵ���Ϣ" << endl;
			wcout << format(L"\n|{:^30}|{:^30}|{:^30}|{:^30}|{:^30}|\n", L"1.�û���", L"2.����", L"3.��ϵ��ʽ", L"4.��ַ", L"5.���������޸�");
			switch (getOperationCode())
			{
			case 1:
				editSuccess = true;
				cout << "���������û�����";
				wcin >> newValue;
				if (newValue.size() > 10) {
					cout << "�û�������" << endl;
					break;
				}
				for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(newValue), wstring2string(userID), "user"))
					for (auto& j : i)
						if (j.first == "name" and j.second == newValue) {
							editSuccess = false;
							cout << "���û����ѱ�ռ��" << endl;
							break;
						}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "name"; }))
							i.second = newValue;
						database.perform("UPDATE user SET name = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
						cout << "�޸��û����ɹ�" << endl;
					}
					catch (const std::exception&)
					{
						cout << "����δ��Ч" << endl;
					}
				}
				else {
					cout << "����δ��Ч" << endl;
				}
				break;
			case 2:
				editSuccess = true;
				cout << "�����������룺";
				wcin >> newValue;
				if (newValue.size() > 20) {
					cout << "�������" << endl;
					break;
				}
				for (auto& i : newValue) {
					if (!isdigit(i) and !islower(i))
					{
						cout << "�����������Сд��ĸ���������" << endl;
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
						cout << "�޸�����ɹ�" << endl;
					}
					catch (const std::exception&)
					{
						cout << "����δ��Ч" << endl;
					}
				}
				else {
					cout << "����δ��Ч" << endl;
				}
				break;
			case 3:
				editSuccess = true;
				cout << "����������ϵ��ʽ��";
				wcin >> newValue;
				if (newValue.size() > 20) {
					cout << "��ϵ��ʽ����" << endl;
					break;
				}
				for (auto& i : newValue | views::filter([](const wchar_t j) {return !isdigit(j); })) {
					cout << "��ϵ��ʽ���������������" << endl;
					editSuccess = false;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "contact"; }))
							i.second = newValue;
						database.perform("UPDATE user SET contact = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
						cout << "�޸���ϵ��ʽ�ɹ�" << endl;
					}
					catch (const std::exception&)
					{
						cout << "����δ��Ч" << endl;
					}
				}
				else {
					cout << "����δ��Ч" << endl;
				}
				break;
			case 4:
				editSuccess = true;
				cout << "�������µ�ַ��";
				wcin >> newValue;
				if (newValue.size() > 40) {
					cout << "��ַ����" << endl;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, wstring> j) {return j.first == "address"; }))
							i.second = newValue;
						database.perform("UPDATE user SET address = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID), wstring2string(userID), "user");
						cout << "�޸ĵ�ַ�ɹ�" << endl;
					}
					catch (const std::exception&)
					{
						cout << "����δ��Ч" << endl;
					}
				}
				else {
					cout << "����δ��Ч" << endl;
				}
				break;
			case 5:
				cout << "�����޸�" << endl;
				break;
			default:
				break;
			}
			break;
		case 3:
			cout << "�������ֵ��";
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
				cout << "����֧����ຬ1λС���ķǸ���ֵ" << endl;
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
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.������Ʒ", L"2.�鿴������Ʒ", L"3.�޸���Ʒ��Ϣ", L"4.�¼���Ʒ", L"5.�鿴��ʷ����", L"6.�����ϲ�˵�");
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

			cout << "��������Ʒ���ƣ�";
			wcin >> tmp;
			if (tmp.length() > 20)
			{
				cout << "��Ʒ���ƹ�����������" << endl;
				break;
			}
			newCommodity.push_back(wstring2string(tmp));

			cout << "������۸����ྫȷ��С�����1λ����";
			wcin >> tmp;
			try
			{
				stod(tmp);
			}
			catch (const std::exception&)
			{
				cout << "����ȷ�����룬������" << endl;
				break;
			}
			newCommodity.push_back(format("{:.1f}", stod(tmp)));

			cout << "������������";
			wcin >> tmp;
			try
			{
				stoi(tmp);
				if (stoi(tmp) <= 0)
					throw "";
			}
			catch (const std::exception&)
			{
				cout << "����ȷ�����룬������" << endl;
				break;
			}
			newCommodity.push_back(format("{}", stoi(tmp)));

			cout << "������200���ڵ���Ʒ������";
			wcin >> tmp;
			tmp = regex_replace(tmp, wregex(L","), L"��");
			if (tmp.length() > 200) {
				cout << "����ȷ�����룬������" << endl;
				break;
			}
			newCommodity.push_back(wstring2string(tmp));

			newCommodity.push_back(id);
			newCommodity.push_back(getCurrentTime());
			newCommodity.push_back(wstring2string(L"������"));

			cout << "��ȷ�ϴ������Ʒ����Ϣ��" << endl << endl;
			cout << "��Ʒ���ƣ�" << newCommodity[1] << endl;
			cout << "��Ʒ�۸�" << newCommodity[2] << endl;
			cout << "��Ʒ������" << newCommodity[3] << endl;
			wcout << L"��Ʒ������" << string2wstring(newCommodity[4]) << endl << endl;

			cout << "�Ƿ������Ʒ������1����ӣ��������������Է������" << endl;
			if (getOperationCode() == 1)
			{
				try
				{
					for (auto& i : newCommodity)
						command += i + ",";
					command.pop_back();
					command += ")";
					database.perform(command, id, "seller");
					cout << "��ӳɹ�" << endl;
				}
				catch (const std::exception&)
				{
					cout << "����δ��Ч" << endl;
				}
			}
			else
			{
				cout << "�������" << endl;
			}

			break;
		case 2:
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allCommodity.end());
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
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
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "��������Ҫ�޸���Ϣ����Ʒ����ƷID��";
			cin >> itemID;
			cout << "��ѡ����Ҫ�����޸ĵ���Ϣ" << endl;
			wcout << format(L"\n|{:^38}|{:^38}|{:^38}|{:^38}|\n", L"1.����", L"2.�۸�", L"3.����", L"4.����");
			switch (getOperationCode())
			{
			case 1:
				cout << "�����������ƣ�";
				wcin >> tmp;
				if (tmp.length() > 20)
				{
					cout << "��Ʒ���ƹ�����������" << endl;
					break;
				}
				updateKey = "name";
				updateValue = wstring2string(tmp);
				break;
			case 2:
				cout << "�������¼۸�";
				wcin >> tmp;
				try
				{
					stod(tmp);
				}
				catch (const std::exception&)
				{
					cout << "����ȷ�����룬������" << endl;
					break;
				}
				updateKey = "price";
				updateValue = format("{:.1f}", stod(tmp));
				break;
			case 3:
				cout << "��������������";
				wcin >> tmp;
				try
				{
					stoi(tmp);
					if (stoi(tmp) <= 0)
						throw "";
				}
				catch (const std::exception&)
				{
					cout << "����ȷ�����룬������" << endl;
					break;
				}
				updateKey = "count";
				updateValue = format("{}", stoi(tmp));
				break;
			case 4:
				cout << "��������������";
				wcin >> tmp;
				tmp = regex_replace(tmp, wregex(L","), L"��");
				if (tmp.length() > 200) {
					cout << "����ȷ�����룬������" << endl;
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
				cout << "��ӳɹ�" << endl;
			}
			catch (const std::exception&)
			{
				cout << "����δ��Ч" << endl;
			}
			break;
		case 4:
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allCommodity.end());
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "��������Ҫ�¼ܵ���Ʒ����ƷID��";
			cin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "state") return j.second != L"������"; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != string2wstring(itemID); } return true; }), allCommodity.end());
			if (allCommodity.empty()) {
				cout << "�޿��¼���Ʒ����ƷIDΪ" << itemID << endl;
				break;
			}
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "��ȷ���Ƿ��¼���Ʒ������1���¼���Ʒ��������������ȡ���¼�\n";
			if (getOperationCode() == 1) {
				try
				{
					database.perform("UPDATE commodity SET state = ���¼� WHERE itemID = " + itemID, id, "seller");
					cout << "�¼ܳɹ�" << endl;
				}
				catch (const std::exception&)
				{
					cout << "����δ��Ч" << endl;
				}
			}
			else
				cout << "�����¼���Ʒ" << endl;
			break;
		case 5:
			allOrder = database.perform("SELECT * FROM order WHERE sellerID CONTAINS " + id, id, "seller");
			allOrder.erase(remove_if(allOrder.begin(), allOrder.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) != id; } return true; }), allOrder.end());
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "����ID", "��ƷID", "���׵���", "����", "����ʱ��", "����ID", "���ID");
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
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.�鿴��Ʒ�б�", L"2.������Ʒ", L"3.������Ʒ", L"4.�鿴��ʷ����", L"5.�鿴��Ʒ��ϸ��Ϣ", L"6.�����ϲ�˵�");
		switch (getOperationCode())
		{
		case 1:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"������"), id, "buyer");
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"��ƷID", L"��Ʒ����", L"�۸�", L"����", L"����ID", L"�ϼ�ʱ��");
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
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"������"), id, "buyer");
			cout << "��������ƷID��";
			wcin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "sellerID") return wstring2string(j.second) == id; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != itemID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "��Ʒ�����ڻ򲻿ɹ���" << endl;
				break;
			}
			cout << "�����빺��������";
			cin >> count;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [count](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "count") return stoi(j.second) < count; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "��Ʒ�������㣬�޷�����" << endl;
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
				cout << "���㣬�޷�����" << endl;
				break;
			}
			try
			{
				if (count == availableCount)
					database.perform("UPDATE commodity SET count = 0, state = " + wstring2string(L"���¼�") + " WHERE itemID = " + wstring2string(itemID), id, "buyer");
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
				cout << "����δ��Ч" << endl;
			}
			break;
		case 3:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"������"), id, "buyer");
			cout << "������ؼ��֣�";
			wcin >> keyword;
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"��ƷID", L"��Ʒ����", L"�۸�", L"����", L"����ID", L"�ϼ�ʱ��");
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
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "����ID", "��ƷID", "���׵���", "����", "����ʱ��", "����ID", "���ID");
			for (auto& line : allOrder)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 5:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS " + wstring2string(L"������"), id, "buyer");
			wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"��ƷID", L"��Ʒ����", L"�۸�", L"����", L"����ID", L"�ϼ�ʱ��");
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
			cout << "��������Ҫ�鿴����ƷID��";
			wcin >> itemID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [itemID](const vector<pair<string, wstring> >& i) { for (auto& j : i) { if (j.first == "itemID") return j.second != itemID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "��Ʒ�����ڻ򲻿ɹ���" << endl;
				break;
			}
			titles = { L"��ƷID", L"��Ʒ����", L"�۸�", L"����", L"����", L"����ID", L"�ϼ�ʱ��", L"����״̬" };
			cout << endl;
			for (auto& i : allCommodity.front())
				wcout << titles.front() << L"��" << i.second << endl, titles.erase(titles.begin());
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
	cout << "�������û�����";
	wcin >> tmp;
	if (tmp.length() > 10) {
		cout << "�û�������" << endl;
		return;
	}
	auto nameFound = false;
	for (auto& i : allUser)
		for (auto& j : i)
			if (j.first == "name")
				nameFound = nameFound or j.second == tmp;
	if (nameFound) {
		cout << "���û����ѱ�ע��" << endl;
		return;
	}
	newUser.push_back(wstring2string(tmp));

	cout << "���������룺";
	wcin >> tmp;
	if (tmp.size() > 20) {
		cout << "�������" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i) and !islower(i))
		{
			cout << "�����������Сд��ĸ���������" << endl;
			return;
		}
	}
	newUser.push_back(wstring2string(tmp));
	
	cout << "��������ϵ��ʽ��";
	wcin >> tmp;
	if (tmp.size() > 20) {
		cout << "��ϵ��ʽ����" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i))
		{
			cout << "��ϵ��ʽ���������������" << endl;
			return;
		}
	}
	newUser.push_back(wstring2string(tmp));

	cout << "�����������ַ��";
	wcin >> tmp;
	if (tmp.size() > 40) {
		cout << "�����ַ����" << endl;
		return;
	}
	newUser.push_back(wstring2string(tmp));

	newUser.push_back("0.0");

	newUser.push_back(wstring2string(L"����"));

	cout << "��ȷ��ע����Ϣ��" << endl << endl;
	wcout << L"�û�����" << string2wstring(newUser[1]) << endl;
	wcout << L"���룺" << string2wstring(newUser[2]) << endl;
	wcout << L"��ϵ��ʽ��" << string2wstring(newUser[3]) << endl;
	wcout << L"�����ַ��" << string2wstring(newUser[4]) << endl;
	cout << endl;

	cout << "����1��ȷ��ע�ᣬ������������ȡ��ע��" << endl;
	if (getOperationCode() == 1) {
		try
		{
			string command = "INSERT INTO user VALUES (";
			for (auto& i : newUser)
				command += i + ",";
			command.pop_back();
			command += ")";
			database.perform(command, "admin", "123456");
			cout << "ע��ɹ��������ϲ�ҳ��" << endl;
		}
		catch (const std::exception&)
		{
			cout << "����δ��Ч" << endl;
		}
	}
	else {
		cout << "����ע��" << endl;
	}
}
