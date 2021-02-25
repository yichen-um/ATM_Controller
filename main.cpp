#include <iostream>
#include "BankAccount.h"
#include "ATMController.h"

using namespace std;

int main() {
    std::cout << "==========Creating a bank database========" << std::endl;
    BankAccount acc_1(123456, 1234, "Jim");
    acc_1.AddAccount(BankAccount::Checking, 0);
    acc_1.AddAccount(BankAccount::Saving, 0);
    
    BankAccount acc_2(654321, 4321, "Mr. Bear");
    acc_2.AddAccount(BankAccount::Checking, 0);
    acc_2.AddAccount(BankAccount::Saving, 0);

    Database bank_database;
    bank_database[acc_1.GetCardNumber()] = std::make_shared<BankAccount>(acc_1);
    bank_database[acc_2.GetCardNumber()] = std::make_shared<BankAccount>(acc_2);
    
    std::cout << "==========Testing ATM controller========" << std::endl;
    ATMController atm;
    atm.GetBankDatabase(std::make_shared<Database>(bank_database));
    atm.Run();
    return 0;
}