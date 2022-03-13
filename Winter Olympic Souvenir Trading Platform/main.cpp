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
		//
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
		for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(name))) {
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
					wcout << format(L"|{:^37}", i.second);
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
				for (auto& i : database.perform("SELECT * FROM user WHERE name CONTAINS " + wstring2string(newValue)))
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
						database.perform("UPDATE user SET name = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
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
						database.perform("UPDATE user SET password = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
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
						database.perform("UPDATE user SET contact = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
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
						database.perform("UPDATE user SET address = " + wstring2string(newValue) + " WHERE userID = " + wstring2string(userID));
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
				database.perform("UPDATE user SET wallet = " + wstring2string(format(L"{:.1f}", stod(oldValue) + stod(newValue))) + " WHERE userID = " + wstring2string(userID));
				database.perform("INSERT INTO recharge VALUES (" + wstring2string(userID) + "," + wstring2string(newValue) + "," + getCurrentTime() + ")");
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
	auto allCommodity = database.perform("SELECT * FROM commodity");
	vector<string> newCommodity;
	while (keepHere) {
		wcout << format(L"\n|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|{:^25}|\n", L"1.������Ʒ", L"2.�鿴������Ʒ", L"3.�޸���Ʒ��Ϣ", L"4.�¼���Ʒ", L"5.�鿴��ʷ����", L"6.�����ϲ�˵�");
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
			tmp = regex_replace(tmp, wregex(L"\."), L"��");
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
					database.perform(command);
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
