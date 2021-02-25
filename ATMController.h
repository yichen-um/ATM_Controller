#ifndef ATM_CONTROLLER_H
#define ATM_CONTROLLER_H

#include <memory>
#include "BankAccount.h"

/*
ATM controller for associated bank account. 
*/

class ATMController{
    public:
    ATMController();
    void Run();
    void GetBankDatabase(const std::shared_ptr<Database> bank_database);
    
    private:
    enum State {
        WaitForCard,
        WaitForPin,
        WaitForAccountSelection,
        WaitForActionSelection,
        WaitForCheckBalance,
        WaitForDeposit,
        WaitForWithdraw,
        Exit
    };
    int GetUserInput();
    bool InsertCard(unsigned int card_number);
    bool VerifyPin(unsigned int pin);
    void SelectAccount(BankAccount::AccountType type);
    void CheckBalance();
    void Deposit(unsigned int amount);
    int Withdraw(unsigned int amount);
    void SetState(const State s);
    State atm_state_;
    bool pin_verified_;
    // Pointers to get access bank's database and current operating account.
    std::shared_ptr<Database> database_ptr;
    std::shared_ptr<BankAccount> account_ptr_;
};

#endif // ATM_CONTROLLER_H
