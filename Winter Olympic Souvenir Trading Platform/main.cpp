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

	cout << "�˳�����\n";
	return 0;
}

bool WelcomePage() {
	int operationCode;
	User user;
	cout << "1.�û���¼  2.�û�ע��  3.�˳�����  4.����Ա��¼\n";
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
		//
		break;
	case 2:
		//
		break;
	case 3:
		cout << "�˳���ӭҳ\n";
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