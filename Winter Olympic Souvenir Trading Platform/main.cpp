#include <iostream>
#include <string>
#include "main.h"
#include "Administator.h"
#include "Database.h"
using namespace std;

Database database;
Administator administator(database);

int main(int argc, char* args[]) {
	while (WelcomePage());

	cout << "�˳�����\n";
	return 0;
}

bool WelcomePage() {
	int operationCode;
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
	case 2:
		//
	case 3:
		cout << "�˳���ӭҳ\n";
		return false;
	case 4:
		AdministatorPage();
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