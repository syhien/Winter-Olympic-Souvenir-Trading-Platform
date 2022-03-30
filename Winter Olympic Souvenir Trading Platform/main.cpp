#include <iostream>
#include <string>
#include "main.h"
#include "Administator.h"
#include "Database.h"
#include "terminal.h"
#include "Calculator.h"
#include <iomanip>
#include <ranges>
#include <sstream>
#include <ctype.h>
#include <regex>
#include <queue>
using namespace std;

Database database;

int main(int argc, char* args[]) {

	Administator administator(&database);

	while (WelcomePage(administator));

	cout << "Exit\n";
	return 0;
}

bool WelcomePage(Administator& administator) {
	int operationCode;
	cout << setw(30) << "1.Log in" << setw(30) << "2.Sign up" << setw(30) << "3.Exit" << setw(30) << "4.Log in as Administrator" << endl;
	while (true)
	{
		operationCode = getOperationCode();
		if (operationCode == 1 or operationCode == 2 or operationCode == 3 or operationCode == 4)
			break;
		else
			cout << "Unknown operation\n";
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
		cout << "Exit Welcome Page\n";
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
	cout << "Input administrator name:";
	cin >> name;
	cout << "Input administrator password:";
	cin >> password;
	while (administator.login(name, password) == false)
	{
		cout << "Administrator's name or password is wrong.\nTry again?\nInput 1 to retry or input others to quit\n";
		if (getOperationCode() != 1)
			return;
		cout << "Input administrator name:";
		cin >> name;
		cout << "Input administrator password:";
		cin >> password;
	}
	cout << "Log in successfully!\n";
	administator.HomePage();
}

void LogIn()
{
	bool checkName = false;
	bool checkPassword = false;
	vector<pair<string, string> > userInfo;
	while (!checkName or !checkPassword)
	{
		string name, password;
		cout << "Input username:";
		cin >> name;
		cout << "Input password:";
		cin >> password;
		for (auto& i : database.perform("SELECT * FROM user WHERE username CONTAINS " + name, "admin", "123456")) {
			for (auto& j : i) {
				if (j.first == "username" and j.second == name)
					checkName = true;
				if (j.first == "password" and j.second == password)
					checkPassword = true;
				if (j.first == "userState" and j.second == "inactive") {
					cout << "The user has been banned." << endl;
					return;
				}
			}
			if (checkName and checkPassword)
				userInfo = i;
		}
		if (!checkName or !checkPassword) {
			cout << "Name or password is wrong.\nTry again?\nInput 1 to retry or input others to quit\n";
			if (getOperationCode() != 1)
				return;
		}
	}
	cout << "Log in successfully\n";
	bool keepHere = true;
	string id;
	for (auto& i : userInfo | views::filter([&](const pair<string, string>& j) {return j.first == "userID"; }))
		id = i.second;
	while (keepHere)
	{
		cout << setw(30) << "1.I am Buyer" << setw(30) << "2.I am Seller" << setw(30) << "3.Manage profile" << setw(30) << "4.Manage blacklist" << setw(30) << "5.Return" << endl;
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
			BlacklistManagePage(userInfo);
			break;
		case 5:
			keepHere = false;
			break;
		default:
			break;
		}
	}
}

void BlacklistManagePage(std::vector<std::pair<std::string, std::string>>& userInfo)
{
	bool keepHere = true;
	string userID;
	for (auto& i : userInfo | views::filter([](const pair<string, string> j) {return j.first == "userID"; }))
		userID = i.second;
	auto allBlacklist = database.perform("SELECT * FROM blacklist", userID, "user");
	auto allUser = database.perform("SELECT * FROM user", "admin", "123456");
	string bannedUserID;
	while (keepHere)
	{
		cout << setw(30) << "1.Check blacklist" << setw(30) << "2.Add blacklist" << setw(30) << "3.Return" << endl;
		switch (getOperationCode())
		{
		case 1:
			allBlacklist = database.perform("SELECT * FROM blacklist", userID, "user");
			cout << setw(30) << "bannedUserID" << setw(30) << "date" << endl;
			for (auto& i : allBlacklist) {
				for (auto& j : i)
					if (j.first != "userID")
						cout << setw(30) << j.second;
				cout << endl;
			}
			break;
		case 2:
			cout << "Input the userID that should be added into blacklist:";
			cin >> bannedUserID;
			allUser = database.perform("SELECT * FROM user", "admin", "123456");
			allUser.erase(remove_if(allUser.begin(), allUser.end(), [bannedUserID](const vector<pair<string, string> >& i) {for (auto& j : i) { if (j.first == "userID") return j.second != bannedUserID; } return false; }), allUser.end());
			if (allUser.empty() or bannedUserID == userID) {
				cout << "User not found or unable to add into blacklist" << endl;
				break;
			}
			cout << "Check the informations" << endl;
			cout << "userID:" + allUser.front()[0].second << endl;
			cout << "username:" + allUser.front()[1].second << endl;
			cout << "Input 1 to add him or her into your blacklist or input others to quit" << endl;
			if (getOperationCode() != 1) {
				cout << "quit" << endl;
				break;
			}
			try
			{
				database.perform("INSERT INTO blacklist VALUES (" + userID + "," + bannedUserID + "," + getCurrentTime() + ")", userID, "user");
				cout << "add successfully" << endl;
			}
			catch (const std::exception&)
			{
				cout << "1gnore 0peration" << endl;
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

void InfoManagePage(std::vector<std::pair<std::string, std::string>>& userInfo)
{
	bool keepHere = true;
	string newValue;
	string oldValue;
	bool editSuccess = false;
	string userID;
	for (auto& i : userInfo | views::filter([newValue](const pair<string, string> j) {return j.first == "userID"; }))
		userID = i.second;
	while (keepHere)
	{
		cout << setw(30) << "1.Check profile" << setw(30) << "2.Edit profile" << setw(30) << "3.Recharge" << setw(30) << "4.Return" << endl;
		switch (getOperationCode())
		{
		case 1:
			cout << setw(30) << "username" << setw(30) << "phone number" << setw(30) << "address" << setw(30) << "balance" << endl;
			for (auto& i : userInfo) {
				if (i.first == "username") {
					cout << setw(30) << i.second;
				}
				else if (i.first == "phoneNumber") {
					cout << setw(30) << i.second;
				}
				else if (i.first == "address") {
					cout << setw(30) << i.second;
				}
				else if (i.first == "balance") {
					try
					{
						if (stod(calculateWallet(userID)) < 0)
							throw "";
						database.perform("UPDATE user SET balance = " + calculateWallet(userID) + " WHERE userID = " + userID, userID, "user");
						cout << setw(30) << calculateWallet(userID);
					}
					catch (const std::exception&)
					{
						cout << endl << "Balance Error" << endl;
					}
				}
			}
			cout << endl;
			break;
		case 2:
			cout << "Choose the information that should be updated" << endl;
			cout << setw(30) << "1.username" << setw(30) << "2.password" << setw(30) << "3.phone number" << setw(30) << "4.address" << setw(30) << "5.Quit" << endl;
			switch (getOperationCode())
			{
			case 1:
				editSuccess = true;
				cout << "Input new username:";
				cin >> newValue;
				if (newValue.size() > 10) {
					cout << "Too long username" << endl;
					break;
				}
				for (auto& i : database.perform("SELECT * FROM user WHERE username CONTAINS " + newValue, userID, "user"))
					for (auto& j : i)
						if (j.first == "username" and j.second == newValue) {
							editSuccess = false;
							cout << "Someone already used this username" << endl;
							break;
						}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, string> j) {return j.first == "username"; }))
							i.second = newValue;
						database.perform("UPDATE user SET username = " + newValue + " WHERE userID = " + userID, userID, "user");
						cout << "Change username successfully" << endl;
					}
					catch (const std::exception&)
					{
						cout << "1gnore 0peration" << endl;
					}
				}
				else {
					cout << "1gnore 0peration" << endl;
				}
				break;
			case 2:
				editSuccess = true;
				cout << "Input new password:";
				cin >> newValue;
				if (newValue.size() > 20) {
					cout << "Too long password" << endl;
					break;
				}
				for (auto& i : newValue) {
					if (!isdigit(i) and !islower(i))
					{
						cout << "Only lowers and digits are allowed" << endl;
						editSuccess = false;
						break;
					}
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, string> j) {return j.first == "password"; }))
							i.second = newValue;
						database.perform("UPDATE user SET password = " + newValue + " WHERE userID = " + userID, userID, "user");
						cout << "Change password successfully" << endl;
					}
					catch (const std::exception&)
					{
						cout << "1gnore 0peration" << endl;
					}
				}
				else {
					cout << "1gnore 0peration" << endl;
				}
				break;
			case 3:
				editSuccess = true;
				cout << "Input new phone number:";
				cin >> newValue;
				if (newValue.size() > 20) {
					cout << "Too long phone number" << endl;
					break;
				}
				for (auto& i : newValue | views::filter([](const auto& j) {return !isdigit(j); })) {
					cout << "Only digits are allowed" << endl;
					editSuccess = false;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, string> j) {return j.first == "phoneNumber"; }))
							i.second = newValue;
						database.perform("UPDATE user SET phoneNumber = " + newValue + " WHERE userID = " + userID, userID, "user");
						cout << "Change phone number successfully" << endl;
					}
					catch (const std::exception&)
					{
						cout << "1gnore 0peration" << endl;
					}
				}
				else {
					cout << "1gnore 0peration" << endl;
				}
				break;
			case 4:
				editSuccess = true;
				cout << "Input new address:";
				cin >> newValue;
				if (newValue.size() > 40) {
					cout << "Too long address" << endl;
					break;
				}
				if (editSuccess) {
					try
					{
						for (auto& i : userInfo | views::filter([newValue](const pair<string, string> j) {return j.first == "address"; }))
							i.second = newValue;
						database.perform("UPDATE user SET address = " + newValue + " WHERE userID = " + userID, userID, "user");
						cout << "Change address successfully" << endl;
					}
					catch (const std::exception&)
					{
						cout << "1gnore 0peration" << endl;
					}
				}
				else {
					cout << "1gnore 0peration" << endl;
				}
				break;
			case 5:
				cout << "Quit" << endl;
				break;
			default:
				break;
			}
			break;
		case 3:
			cout << "Input recharge:";
			cin >> newValue;
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
				for (auto& i : userInfo | views::filter([newValue](const pair<string, string>& j) {return j.first == "balance"; })) {
					oldValue = i.second;
					i.second = to_string(stod(oldValue) + stod(newValue));
				}
				database.perform("UPDATE user SET balance = " + to_string(stod(oldValue) + stod(newValue)) + " WHERE userID = " + userID, userID, "user");
				database.perform("INSERT INTO recharge VALUES (" + userID + "," + newValue + "," + getCurrentTime() + ")", userID, "user");
			}
			catch (const std::exception&)
			{
				cout << "Only abc and abc.d are supported" << endl;
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
	string tmp;
	string command;
	auto allCommodity = std::vector<std::vector<std::pair<std::string, std::string>>>();
	auto allOrder = std::vector<std::vector<std::pair<std::string, std::string>>>();
	vector<string> newCommodity;
	string commodityID;
	string updateKey, updateValue;
	stringstream tmpStrStream;
	while (keepHere) {
		cout << setw(30) << "1.New commodity" << setw(30) << "2.Check commodity" << setw(30) << "3.Edit commodity" << setw(30) << "4.Discontinue commodity" << setw(30) << "5.Check order" << setw(30) << "6.Return" << endl;
		switch (getOperationCode())
		{
		case 1:
			command = "INSERT INTO commodity VALUES (";
			newCommodity.clear();
			allCommodity = database.perform("SELECT * FROM commodity", "admin", "123456");
			for (int i = 1; i < 999; i++) {
				bool existed = false;
				tmpStrStream.clear();
				tmpStrStream.str("");
				tmpStrStream << "M";
				tmpStrStream << setw(3) << setfill('0') << setiosflags(ios::right) << i;
				for (auto& j : allCommodity)
					for (auto& k : j | views::filter([i](const pair<string, string> l) {return l.first == "commodityID"; })) {
						existed = existed or k.second == tmpStrStream.str();
					}
				if (!existed) {
					newCommodity.push_back(tmpStrStream.str());
					break;
				}
			}

			cout << "Input name:";
			cin >> tmp;
			if (tmp.length() > 20)
			{
				cout << "Too long name" << endl;
				break;
			}
			newCommodity.push_back(tmp);

			cout << "Input price:";
			cin >> tmp;
			try
			{
				stod(tmp);
			}
			catch (const std::exception&)
			{
				cout << "Wrong format" << endl;
				break;
			}
			tmpStrStream.clear();
			tmpStrStream.str("");
			tmpStrStream.setf(ios::fixed);
			tmpStrStream << setprecision(1) << stod(tmp);
			newCommodity.push_back(tmpStrStream.str());

			cout << "Input number:";
			cin >> tmp;
			try
			{
				stoi(tmp);
				if (stoi(tmp) <= 0)
					throw "";
			}
			catch (const std::exception&)
			{
				cout << "Wrong format" << endl;
				break;
			}
			tmpStrStream.clear();
			tmpStrStream.str("");
			tmpStrStream << stoi(tmp);
			newCommodity.push_back(tmpStrStream.str());

			cout << "Input description:";
			cin.clear();
			cin.ignore();
			getline(cin, tmp);
			tmp = regex_replace(tmp, regex(","), "_");
			tmp = regex_replace(tmp, regex(" "), "_");
			if (tmp.length() > 200) {
				cout << "Too long description" << endl;
				break;
			}
			newCommodity.push_back(tmp);

			newCommodity.push_back(id);
			newCommodity.push_back(getCurrentTime());
			newCommodity.push_back("onSale");

			cout << "Make sure everything is correct:" << endl << endl;
			cout << "name:" << newCommodity[1] << endl;
			cout << "price:" << newCommodity[2] << endl;
			cout << "number:" << newCommodity[3] << endl;
			cout << "description:" << newCommodity[4] << endl << endl;

			cout << "Add commodity? Input 1 to add or input others to quit" << endl;
			if (getOperationCode() == 1)
			{
				try
				{
					for (auto& i : newCommodity)
						command += i + ",";
					command.pop_back();
					command += ")";
					database.perform(command, id, "seller");
					cout << "Add successfully" << endl;
				}
				catch (const std::exception&)
				{
					cout << "1gnore 0peration" << endl;
				}
			}
			else
			{
				cout << "1gnore 0peration" << endl;
			}

			break;
		case 2:
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != id; } return true; }), allCommodity.end());
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "description" << setw(30) << "sellerID" << setw(30) << "addedDate" << setw(30) << "state" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 3:
			updateKey = "dont";
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != id; } return true; }), allCommodity.end());
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "description" << setw(30) << "sellerID" << setw(30) << "addedDate" << setw(30) << "state" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Input the commodityID of the commodity that should be updated:";
			cin >> commodityID;
			cout << "Edit:" << endl;
			cout << setw(30) << "1.name" << setw(30) << "2.price" << setw(30) << "3.number" << setw(30) << "4.description" << endl;
			switch (getOperationCode())
			{
			case 1:
				cout << "Input new name:";
				cin >> tmp;
				if (tmp.length() > 20)
				{
					cout << "Too long name" << endl;
					break;
				}
				updateKey = "commodityName";
				updateValue = tmp;
				break;
			case 2:
				cout << "Input new price:";
				cin >> tmp;
				try
				{
					stod(tmp);
				}
				catch (const std::exception&)
				{
					cout << "Wrong format" << endl;
					break;
				}
				tmpStrStream.clear();
				tmpStrStream.str("");
				tmpStrStream.setf(ios::fixed);
				tmpStrStream << setprecision(1) << stod(tmp);
				updateKey = "price";
				updateValue = tmpStrStream.str();
				break;
			case 3:
				cout << "Input new number:";
				cin >> tmp;
				try
				{
					stoi(tmp);
					if (stoi(tmp) <= 0)
						throw "";
				}
				catch (const std::exception&)
				{
					cout << "Wrong format" << endl;
					break;
				}
				tmpStrStream.clear();
				tmpStrStream.str("");
				tmpStrStream << stoi(tmp);
				updateKey = "number";
				updateValue = tmpStrStream.str();
				break;
			case 4:
				cout << "Input new description:";
				cin.clear();
				cin.ignore();
				getline(cin, tmp);
				tmp = regex_replace(tmp, regex(","), "_");
				tmp = regex_replace(tmp, regex(" "), "_");
				if (tmp.length() > 200) {
					cout << "Wrong format" << endl;
					break;
				}
				updateKey = "description";
				updateValue = tmp;
				break;
			default:
				break;
			}
			if (updateKey == "dont")
				break;
			try
			{
				command = "UPDATE commodity SET " + updateKey + " = " + updateValue + " WHERE commodityID = " + commodityID;
				database.perform(command, id, "seller");
				cout << "Edit successfully" << endl;
			}
			catch (const std::exception&)
			{
				cout << "1gnore 0peration" << endl;
			}
			break;
		case 4:
			allCommodity = database.perform("SELECT * FROM commodity WHERE sellerID CONTAINS " + id, id, "seller");
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != id; } return true; }), allCommodity.end());
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "description" << setw(30) << "sellerID" << setw(30) << "addedDate" << setw(30) << "state" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Input the commodityID of the commodity that should be uncontinued:";
			cin >> commodityID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "state") return j.second != "onSale"; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [commodityID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "commodityID") return j.second != commodityID; } return true; }), allCommodity.end());
			if (allCommodity.empty()) {
				cout << "No commodity's commodityID equals to " << commodityID << endl;
				break;
			}
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "description" << setw(30) << "sellerID" << setw(30) << "addedDate" << setw(30) << "state" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Uncontinue it? Input 1 to uncontinue it or input others to quit\n";
			if (getOperationCode() == 1) {
				try
				{
					database.perform("UPDATE commodity SET state = removed WHERE commodityID = " + commodityID, id, "seller");
					cout << "Uncontinue successfully" << endl;
				}
				catch (const std::exception&)
				{
					cout << "1gnore 0peration" << endl;
				}
			}
			else
				cout << "Quit" << endl;
			break;
		case 5:
			allOrder = database.perform("SELECT * FROM order WHERE sellerID CONTAINS " + id, id, "seller");
			allOrder.erase(remove_if(allOrder.begin(), allOrder.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != id; } return true; }), allOrder.end());
			cout << setw(30) << "orderID" << setw(30) << "commodityID" << setw(30) << "unitPrice" << setw(30) << "number" << setw(30) << "date" << setw(30) << "sellerID" << setw(30) << "buyerID" << endl;
			for (auto& line : allOrder)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
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
	auto allCommodity = std::vector<std::vector<std::pair<std::string, std::string>>>();
	auto allOrder = std::vector<std::vector<std::pair<std::string, std::string>>>();
	string commodityID;
	int count;
	int availableCount;
	double price;
	string newOrderID;
	string sellerID;
	string keyword;
	vector<string> titles;
	stringstream tmpStrStream;
	while (keepHere)
	{
		cout << setw(30) << "1.View commoditys" << setw(30) << "2.Buy commodity" << setw(30) << "3.Search" << setw(30) << "4.View orders" << setw(30) << "5.View details of commodity" << setw(30) << "6.Return" << endl;
		switch (getOperationCode())
		{
		case 1:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS onSale", id, "buyer");
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "sellerID" << setw(30) << "addedDate" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 2:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS onSale", id, "buyer");
			cout << "Input commodityID:";
			cin >> commodityID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second == id; } return true; }), allCommodity.end());
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [commodityID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "commodityID") return j.second != commodityID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "Not found or unable to buy" << endl;
				break;
			}
			cout << "Input number:";
			cin >> count;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [count](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "number") return stoi(j.second) < count; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "No enough commodity" << endl;
				break;
			}
			for (auto& i : allCommodity.front())
				if (i.first == "price")
					price = stod(i.second);
				else if (i.first == "number")
					availableCount = stoi(i.second);
				else if (i.first == "sellerID")
					sellerID = i.second;
			if (stod(calculateWallet(id)) < price * count) {
				cout << "No enough balance" << endl;
				break;
			}
			try
			{
				if (count == availableCount)
					database.perform("UPDATE commodity SET number = 0 , state = removed WHERE commodityID = " + commodityID, id, "buyer");
				else
					database.perform("UPDATE commodity SET number = " + to_string(availableCount - count) + " WHERE commodityID = " + commodityID, id, "buyer");
				allOrder = database.perform("SELECT * FROM order", id, "buyer");
				for (int i = 1; i < 999; i++) {
					bool existed = false;
					tmpStrStream.clear();
					tmpStrStream.str("");
					tmpStrStream << "T";
					tmpStrStream << setw(3) << setfill('0') << setiosflags(ios::right) << i;
					for (auto& j : allOrder)
						for (auto& k : j | views::filter([i](const pair<string, string> l) {return l.first == "orderID"; })) {
							existed = existed or k.second == tmpStrStream.str();
						}
					if (!existed) {
						newOrderID = tmpStrStream.str();
						break;
					}
				}
				database.perform("INSERT INTO order VALUES (" + newOrderID + "," + commodityID + "," + to_string(price) + "," + to_string(count) + "," + getCurrentTime() + "," + sellerID + "," + id + ")", id, "buyer");
				database.perform("UPDATE user SET balance = " + calculateWallet(id) + " WHERE userID = " + id, id, "buyer");
				cout << "Buy successfully!" << endl;
			}
			catch (const std::exception&)
			{
				cout << "1gnore 0peration" << endl;
			}
			break;
		case 3:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS onSale", id, "buyer");
			cout << "Input keyword:";
			cin >> keyword;
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "price" << setw(30) << "number" << setw(30) << "sellerID" << setw(30) << "addedDate" << endl;
			for (auto& line : allCommodity | views::filter([keyword](const vector<pair<string, string> >& i) {for (auto& j : i) { if (j.second.find(keyword) != string::npos) return true; } return false; })) {
				cout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 4:
			allOrder = database.perform("SELECT * FROM order WHERE buyerID CONTAINS " + id, id, "buyer");
			allOrder.erase(remove_if(allOrder.begin(), allOrder.end(), [id](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "buyerID") return j.second != id; } return true; }), allOrder.end());
			cout << setw(30) << "orderID" << setw(30) << "commodityID" << setw(30) << "unitPrice" << setw(30) << "number" << setw(30) << "date" << setw(30) << "sellerID" << setw(30) << "buyerID" << endl;
			for (auto& line : allOrder)
			{
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 5:
			allCommodity = database.perform("SELECT * FROM commodity WHERE state CONTAINS onSale", id, "buyer");
			cout << setw(30) << "commodityID" << setw(30) << "name" << setw(30) << "unitPrice" << setw(30) << "number" << setw(30) << "sellerID" << "addedDate" << endl;
			for (auto& line : allCommodity)
			{
				cout << endl;
				for (auto& i : line)
					if (i.first == "description" or i.first == "state")
						continue;
					else
						cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Input the commodityID that shoule be viewed:";
			cin >> commodityID;
			allCommodity.erase(remove_if(allCommodity.begin(), allCommodity.end(), [commodityID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "commodityID") return j.second != commodityID; } return true; }), allCommodity.end());
			if (allCommodity.size() == 0) {
				cout << "No such commodity" << endl;
				break;
			}
			titles = { "commodityID", "name", "unitPrice", "number", "description", "sellerID", "addedDate", "state" };
			cout << endl;
			for (auto& i : allCommodity.front())
				cout << titles.front() << ":" << i.second << endl, titles.erase(titles.begin());
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
	allRecharge.erase(remove_if(allRecharge.begin(), allRecharge.end(), [userID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "userID") return j.second != userID; } return true; }), allRecharge.end());
	auto allBuyOrder = database.perform("SELECT * FROM order WHERE buyerID CONTAINS " + userID, userID, "buyer");
	allBuyOrder.erase(remove_if(allBuyOrder.begin(), allBuyOrder.end(), [userID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "buyerID") return j.second != userID; } return true; }), allBuyOrder.end());
	auto allSellOrder = database.perform("SELECT * FROM order WHERE sellerID CONTAINS " + userID, userID, "seller");
	allSellOrder.erase(remove_if(allSellOrder.begin(), allSellOrder.end(), [userID](const vector<pair<string, string> >& i) { for (auto& j : i) { if (j.first == "sellerID") return j.second != userID; } return true; }), allSellOrder.end());
	string command = "";
	map<string, int> numbers;
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
		string price;
		for (auto& j : i)
			if (j.first == "number")
				count = stoi(j.second);
			else if (j.first == "unitPrice")
				price = "-" + j.second;
		if (numbers.contains(price))
			numbers[price] += count;
		else
			numbers[price] = count;
	}
	for (auto& i : allSellOrder) {
		int count;
		string price;
		for (auto& j : i)
			if (j.first == "number")
				count = stoi(j.second);
			else if (j.first == "unitPrice")
				price = j.second;
		if (numbers.contains(price))
			numbers[price] += count;
		else
			numbers[price] = count;
	}
	map<int, vector<string>> countNumbers;
	for (auto& i : numbers)
		countNumbers[i.second].push_back(i.first);
	for (auto& i : countNumbers) {
		if (command.length() == 0)
			command += to_string(i.first) + " * ";
		else
			command += " + " + to_string(i.first) + "*";
		command += "(";

		string subCommand = "";
		for (auto& j : i.second)
			if (subCommand.length() == 0)
				subCommand += j;
			else
				subCommand += "+(" + j + ")";
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
	stringstream tmpStrStream;
	for (int i = 1; i < 999; i++) {
		bool existed = false;
		tmpStrStream.clear();
		tmpStrStream.str("");
		tmpStrStream << "U";
		tmpStrStream << setw(3) << setfill('0') << setiosflags(ios::right) << i;
		for (auto& j : allUser)
			for (auto& k : j | views::filter([i](const pair<string, string>& l) {return l.first == "userID"; })) {
				existed = existed or k.second == tmpStrStream.str();
			}
		if (!existed) {
			newUser.push_back(tmpStrStream.str());
			break;
		}
	}

	string tmp;
	cout << "Input username:";
	cin >> tmp;
	if (tmp.length() > 10) {
		cout << "too long username" << endl;
		return;
	}
	auto nameFound = false;
	for (auto& i : allUser)
		for (auto& j : i)
			if (j.first == "username")
				nameFound = nameFound or j.second == tmp;
	if (nameFound) {
		cout << "Someone already used this username" << endl;
		return;
	}
	newUser.push_back(tmp);

	cout << "Input password:";
	cin >> tmp;
	if (tmp.size() > 20) {
		cout << "Too long password" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i) and !islower(i))
		{
			cout << "Only lowers and digits are allowed" << endl;
			return;
		}
	}
	newUser.push_back(tmp);
	
	cout << "Input phone number:";
	cin >> tmp;
	if (tmp.size() > 20) {
		cout << "Too long phone number" << endl;
		return;
	}
	for (auto& i : tmp) {
		if (!isdigit(i))
		{
			cout << "Only digits are allowed" << endl;
			return;
		}
	}
	newUser.push_back(tmp);

	cout << "Input address:";
	cin >> tmp;
	if (tmp.size() > 40) {
		cout << "Too long address" << endl;
		return;
	}
	newUser.push_back(tmp);

	newUser.push_back("0.0");

	newUser.push_back("active");

	cout << "Check informations:" << endl << endl;
	cout << "username:" << newUser[1] << endl;
	cout << "password:" << newUser[2] << endl;
	cout << "phone number:" << newUser[3] << endl;
	cout << "address:" << newUser[4] << endl;
	cout << endl;

	cout << "Input 1 to sign up or input others to quit" << endl;
	if (getOperationCode() == 1) {
		try
		{
			string command = "INSERT INTO user VALUES (";
			for (auto& i : newUser)
				command += i + ",";
			command.pop_back();
			command += ")";
			database.perform(command, "admin", "123456");
			cout << "Sign up successfully" << endl;
		}
		catch (const std::exception&)
		{
			cout << "1gnore 0peration" << endl;
		}
	}
	else {
		cout << "Quit" << endl;
	}
}
