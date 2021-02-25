#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <memory>

/*
Class for bank account. Two default types of account "Checking account" and "Saving account" are included. 
*/
class BankAccount {
    public:
    enum AccountType {
        Checking = 1,
        Saving
    };
    BankAccount(unsigned int card_number, unsigned int pin, const std::string& name);
    unsigned int GetCardNumber() const;
    std::string GetUserName() const;
    void AddAccount(const AccountType type, const unsigned int init_amount);
    bool VerifyPin(const unsigned pin);
    bool SelectAccount(AccountType type);
    std::string GetCurrentAccountName();
    int GetBalance();
    void Deposit(unsigned int amount);
    unsigned int WithDraw(unsigned int amount);

    private:
    std::string user_name_;
    unsigned int card_number_;
    unsigned int pin_;
    AccountType cur_acc_type_;
    std::unordered_map<int, int> balances_;
    std::unordered_map<int, std::string> account_names_;
};

typedef std::unordered_map<unsigned int, std::shared_ptr<BankAccount> > Database;

#endif // BANK_ACCOUNT_H