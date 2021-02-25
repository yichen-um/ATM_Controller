#include "BankAccount.h"

BankAccount::BankAccount(unsigned int card_number, unsigned int pin, const std::string& name) : card_number_(card_number), pin_(pin), user_name_(name) {
    account_names_[Checking] = "Checking account";
    account_names_[Saving] = "Saving account";
    std::cout << "A bank account has been created." << std::endl;
    std::cout << "  - User name: " << user_name_ << std::endl;
    std::cout << "  - Card number: " << card_number_ << std::endl;
    std::cout << "  - Pin number: " << pin_ << std::endl;
}

unsigned int BankAccount::GetCardNumber() const {
    return card_number_;
}

std::string BankAccount::GetUserName() const {
    return user_name_;
}

void BankAccount::AddAccount(const AccountType type, const unsigned int init_amount) {
    if(balances_[type]) {
        std::cout << "Same type account already exist." << std::endl;
        return;
    }
    balances_[type] = init_amount;
    return;
}

bool BankAccount::VerifyPin(const unsigned pin) {
    return pin_ == pin;
}

bool BankAccount::SelectAccount(AccountType type) {
    if(balances_.find(type) == balances_.end()) {
        return false;
    }
    cur_acc_type_ = type;
    return true;
}

std::string BankAccount::GetCurrentAccountName() {
    return account_names_[cur_acc_type_];
}

int BankAccount::GetBalance() {
    return balances_[cur_acc_type_];
}

void BankAccount::Deposit(unsigned int amount) {
    balances_[cur_acc_type_] += amount;
}

unsigned int BankAccount::WithDraw(unsigned int amount) {
    if(amount > balances_[cur_acc_type_]) {
        return 0;
    }
    balances_[cur_acc_type_] -= amount;
    return amount;
}
