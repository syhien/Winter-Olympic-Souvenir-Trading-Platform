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
			handleCommodity();
			break;
		case 2:
			handleOrder();
			break;
		case 3:
			handleUser();
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

void Administator::handleCommodity()
{
	bool keepHere = true;
	auto allCommodity = __database->perform("SELECT * FROM commodity");
	wstring keyword;
	while (keepHere) {
		wcout << format(L"\n|{:^37}|{:^37}|{:^37}|{:^37}|\n", L"1.�鿴������Ʒ", L"2.������Ʒ", L"3.�¼���Ʒ", L"4.�����ϲ�˵�");
		auto operationCode = getOperationCode();
		allCommodity = __database->perform("SELECT * FROM commodity");
		switch (operationCode)
		{
		case 1:
			cout << format("|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 2:
			cout << "������ؼ��ʣ�";
			wcin >> keyword;
			cout << format("|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto line : allCommodity | views::filter([keyword](const vector<pair<string, wstring> > i) {for (auto j : i) { if (j.second.find(keyword) != wstring::npos) return true; } return false; })) {
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			break;
		case 3:
			cout << format("|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|{:^19}|\n", "��ƷID", "����", "�۸�", "����", "����", "����ID", "�ϼ�ʱ��", "��Ʒ״̬");
			for (auto& line : allCommodity)
			{
				for (auto& i : line)
					wcout << format(L"|{:^19}", i.second);
				wcout << "|\n";
			}
			cout << "��������¼ܵ���Ʒ����ƷID��";
			wcin >> keyword;
			try
			{
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

void Administator::handleOrder()
{
}

void Administator::handleUser()
{
}
