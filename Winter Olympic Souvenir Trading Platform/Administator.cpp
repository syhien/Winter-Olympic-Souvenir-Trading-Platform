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
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.��Ʒ����", L"2.��������", L"3.�û�����", L"4.�ǳ�");
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
			cout << "����Ա�ǳ�\n";
			login = false;
			break;
		default:
			cout << "�޴˲���\n";
			break;
		}
	}
}

string wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
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
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.�鿴������Ʒ", L"2.������Ʒ", L"3.�¼���Ʒ", L"4.�����ϲ�˵�");
		auto operationCode = getOperationCode();
		allCommodity = __database->perform("SELECT * FROM commodity");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << "������ؼ��ʣ�";
			wcin >> keyword;
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto line : allCommodity | views::filter([keyword](const vector<pair<string, wstring> > i) {for (auto j : i) { if (j.second.find(keyword) != wstring::npos) return true; } return false; })) {
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 3:
			cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "��������¼ܵ���Ʒ����ƷID��";
			wcin >> keyword;
			try
			{
				selectedCommodity = __database->perform("SELECT * FROM commodity WHERE itemID CONTAINS " + wstring2string(keyword));
				cout << format("\n|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
				for (auto& line : selectedCommodity)
				{
					wcout << endl;
					for (auto& i : line)
						wcout << format(L"|{:^19}", i.second);
					wcout << "|\n";
				}
				cout << "��ȷ���Ƿ��¼���Ʒ������1���¼���Ʒ��������������ȡ���¼�\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "��ȡ���¼�" << endl;
					break;
				}
				__database->perform("UPDATE commodity SET state = ���¼� WHERE itemID = " + wstring2string(keyword));
				cout << "�¼ܳɹ�\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "����δ��Ч\n";
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
		wcout << format(L"\n|{:^50}|{:^50}|{:^50}|\n", L"1.�鿴���ж���", L"2.��������", L"3.�����ϲ�˵�");
		auto operationCode = getOperationCode();
		allOrder = __database->perform("SELECT * FROM order");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "����ID", "��ƷID", "���׵���", "����", "����ʱ��", "����ID", "���ID");
			for (auto& line : allOrder)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << "������ؼ��ʣ�";
			wcin >> keyword;
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "����ID", "��ƷID", "���׵���", "����", "����ʱ��", "����ID", "���ID");
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
		wcout << format(L"\n|{:^50}|{:^50}|{:^50}|\n", L"1.�鿴�����û�", L"2.����û�", L"3.�����ϲ�˵�");
		auto operationCode = getOperationCode();
		allUser = __database->perform("SELECT * FROM user");
		switch (operationCode)
		{
		case 1:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "�û�ID", "�û���", "����", "��ϵ��ʽ", "��ַ", "Ǯ�����", "�û�״̬");
			for (auto& line : allUser)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "�û�ID", "�û���", "����", "��ϵ��ʽ", "��ַ", "Ǯ�����", "�û�״̬");
			for (auto& line : allUser)
			{
				wcout << endl;
				for (auto& i : line)
					wcout << format(L"|{:^21}", i.second);
				wcout << "|\n";
			}
			cout << "�������������û����û�ID��";
			wcin >> keyword;
			try
			{
				selectedUser = __database->perform("SELECT * FROM user WHERE userID CONTAINS " + wstring2string(keyword));
				cout << format("\n|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|{:^21}|\n", "�û�ID", "�û���", "����", "��ϵ��ʽ", "��ַ", "Ǯ�����", "�û�״̬");
				for (auto& line : selectedUser)
				{
					wcout << endl;
					for (auto& i : line)
						wcout << format(L"|{:^21}", i.second);
					wcout << "|\n";
				}
				cout << "��ȷ���Ƿ����û�������1�Է���û���������������ȡ�����\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "��ȡ�����" << endl;
					break;
				}
				__database->perform("UPDATE user SET state = ��� WHERE userID = " + wstring2string(keyword));
				cout << "����ɹ�\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "����δ��Ч\n";
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
