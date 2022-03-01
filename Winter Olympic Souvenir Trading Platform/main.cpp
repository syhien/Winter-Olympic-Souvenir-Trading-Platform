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
#include <Windows.h>
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
		logIn();
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

static string wstring2string(wstring wstr)
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

void logIn()
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
}

void signUp()
{
}
