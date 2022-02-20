#include "Administator.h"
#include "terminal.h"
using namespace std;

Administator::Administator(Database& database)
{
	__database = database;
}

bool Administator::login(std::string name, std::string password)
{
	return name == __name and password == __password;
}

void Administator::HomePage()
{
	cout << "admin home page, todo\n";
}
