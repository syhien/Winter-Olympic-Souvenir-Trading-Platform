#include "Administator.h"
#include "terminal.h"
#include <ranges>
#include <iomanip>
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
		cout << setw(30) << "1.Commodity operation" << setw(30) << "2.Order operation" << setw(30) << "3.User operation" << setw(30) << "4.Log out" << endl;
		auto operationCode = getOperationCode();
		switch (operationCode)
		{
		case 1:
			__handleCommodityPage();
			break;
		case 2:
			__handleOrderPage();
			break;
		case 3:
			__handleUserPage();
			break;
		case 4:
			cout << "Administrator log out\n";
			login = false;
			break;
		default:
			cout << "Unknown operation\n";
			break;
		}
	}
}

void Administator::__handleCommodityPage()
{
	bool keepHere = true;
	auto allCommodity = __database->perform("SELECT * FROM commodity", "admin", "123456");
	auto selectedCommodity = allCommodity;
	int tmpInt;
	string keyword;
	while (keepHere) {
		cout << setw(30) << "1.Check all commodities" << setw(30) << "2.Search for commodity" << setw(30) << "3.Uncontinue commodity" << setw(30) << "4.Return" << endl;
		auto operationCode = getOperationCode();
		allCommodity = __database->perform("SELECT * FROM commodity", "admin", "123456");
		switch (operationCode)
		{
		case 1:
			for (auto& i : { "commodityID", "commodityName","price","number","description", "sellerID", "addedDate", "state" })
				cout << setw(30) << i;
			cout << endl;
			for (auto& line : allCommodity)
			{
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 2:
			cout << "Input keyword:";
			cin >> keyword;
			for (auto& i : { "commodityID", "commodityName","price","number","description", "sellerID", "addedDate", "state" })
				cout << setw(30) << i;
			cout << endl;
			for (auto line : allCommodity | views::filter([keyword](const vector<pair<string, string> > i) {for (auto j : i) { if (j.second.find(keyword) != string::npos) return true; } return false; })) {
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 3:
			for (auto& i : { "commodityID", "commodityName","price","number","description", "sellerID", "addedDate", "state" })
				cout << setw(30) << i;
			for (auto& line : allCommodity)
			{
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Input the commodityID of the commodity that should be uncoutinued:";
			cin >> keyword;
			try
			{
				selectedCommodity = __database->perform("SELECT * FROM commodity WHERE commodityID CONTAINS " + keyword, "admin", "123456");
				for (auto& i : { "commodityID", "commodityName","price","number","description", "sellerID", "addedDate", "state" })
					cout << setw(30) << i;
				cout << endl;
				for (auto& line : selectedCommodity)
				{
					for (auto& i : line)
						cout << setw(30) << i.second;
					cout << endl;
				}
				cout << "Input 1 to uncontinue it or input others to quit\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "Quit" << endl;
					break;
				}
				__database->perform("UPDATE commodity SET state = removed WHERE commodityID = " + keyword, "admin", "123456");
				cout << "Uncontinue successfully\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "1gnore 0peration\n";
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

void Administator::__handleOrderPage()
{
	bool keepHere = true;
	auto allOrder = __database->perform("SELECT * FROM order", "admin", "123456");
	string keyword;
	while (keepHere) {
		cout << setw(30) << "1.Check all orders" << setw(30) << "2.Search for order" << setw(30) << "3.Return" << endl;
		auto operationCode = getOperationCode();
		allOrder = __database->perform("SELECT * FROM order", "admin", "123456");
		switch (operationCode)
		{
		case 1:
			for (auto& i : { "orderID","commodityID","unitPrice","number","date","sellerID","buyerID" })
				cout << setw(30) << i;
			cout << endl;
			for (auto& line : allOrder)
			{
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 2:
			cout << "Input keyword:";
			cin >> keyword;
			for (auto& i : { "orderID","commodityID","unitPrice","number","date","sellerID","buyerID" })
				cout << setw(30) << i;
			cout << endl;
			for (auto line : allOrder | views::filter([keyword](const vector<pair<string, string> > i) {for (auto j : i) { if (j.second.find(keyword) != string::npos) return true; } return false; })) {
				cout << endl;
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
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

void Administator::__handleUserPage()
{
	bool keepHere = true;
	auto allUser = __database->perform("SELECT * FROM user", "admin", "123456");
	auto selectedUser = allUser;
	string keyword;
	int tmpInt;
	while (keepHere) {
		cout << setw(30) << "1.Check all users" << setw(30) << "2.Ban user" << setw(30) << "3.Return" << endl;
		auto operationCode = getOperationCode();
		allUser = __database->perform("SELECT * FROM user", "admin", "123456");
		switch (operationCode)
		{
		case 1:
			for (auto& i : { "userID","username","password","phoneNumber","address","balance","userState" })
				cout << setw(30) << i;
			cout << endl;
			for (auto& line : allUser)
			{
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			break;
		case 2:
			for (auto& i : { "userID","username","password","phoneNumber","address","balance","userState" })
				cout << setw(30) << i;
			cout << endl;
			for (auto& line : allUser)
			{
				for (auto& i : line)
					cout << setw(30) << i.second;
				cout << endl;
			}
			cout << "Input the userID of the user who should be banned:";
			cin >> keyword;
			try
			{
				selectedUser = __database->perform("SELECT * FROM user WHERE userID CONTAINS " + keyword, "admin", "123456");
				for (auto& i : { "userID","username","password","phoneNumber","address","balance","userState" })
					cout << setw(30) << i;
				cout << endl;
				for (auto& line : selectedUser)
				{
					for (auto& i : line)
						cout << setw(30) << i.second;
					cout << endl;
				}
				cout << "Input 1 to ban the user or input others to quit\n";
				tmpInt = getOperationCode();
				if (tmpInt != 1) {
					cout << "Quit" << endl;
					break;
				}
				__database->perform("UPDATE user SET userState = inactive WHERE userID = " + keyword, "admin", "123456");
				cout << "Ban successfully\n";
			}
			catch (const std::exception& e)
			{
				cout << e.what() << endl;
				cout << "1gnore 0peration\n";
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
