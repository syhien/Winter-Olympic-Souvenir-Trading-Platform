#pragma once
#include "Administator.h"
bool WelcomePage(Administator& administator);
void AdministatorPage(Administator& administator);
void LogIn();
void SignUp();
void InfoManagePage(std::vector<std::pair<std::string, std::wstring> >& userInfo);
void SellerPage();
void BuyerPage();