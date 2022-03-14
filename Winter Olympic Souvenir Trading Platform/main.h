#pragma once
#include "Administator.h"
bool WelcomePage(Administator& administator);
void AdministatorPage(Administator& administator);
void LogIn();
void SignUp();
void InfoManagePage(std::vector<std::pair<std::string, std::wstring> >& userInfo);
void SellerPage(std::string id);
void BuyerPage(std::string id);
std::string calculateWallet(std::string userID);