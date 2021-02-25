#include "ATMController.h"
#include <iostream>
#include <limits>

ATMController::ATMController() {
    pin_verified_ = false;
    account_ptr_ = nullptr;
    atm_state_ = WaitForCard;
}

/*
Run operation with state machine with defined services.
*/
void ATMController::Run() {
    while(true) {
        switch (atm_state_) {
            case WaitForCard: {
                std::cout << "Please insert a card by typing a card number: ";
                unsigned int input = GetUserInput();
                if(InsertCard(input)) {
                    SetState(WaitForPin);
                } else {
                    SetState(WaitForCard);
                };
                break;
            }
            case WaitForPin: {
                std::cout << "Please type your pin number: ";
                int input = GetUserInput();
                if(VerifyPin(input)){
                    SetState(WaitForAccountSelection);
                } else {
                    SetState(WaitForPin);
                }
                std::cout << std::endl;
                break;
            }
            case WaitForAccountSelection: {
                std::string user_name = account_ptr_->GetUserName();
                std::cout << "Welcome, "<< user_name << "!"<< std::endl;
                std::cout << "Please choose an account: " << std::endl;
                std::cout << "1. Checking account" << std::endl;
                std::cout << "2. Saving account" << std::endl;
                std::cout << "0. Exit" << std::endl;
                std::cout << "Entered option:";                
                unsigned int input = GetUserInput();
                switch (input) {
                    case 0: {
                        SetState(Exit); break;                        
                    }
                    case 1: {
                        SelectAccount(static_cast<BankAccount::AccountType>(input));
                        SetState(WaitForActionSelection); break;
                    }
                    case 2: {
                        SelectAccount(static_cast<BankAccount::AccountType>(input));
                        SetState(WaitForActionSelection); break;
                    }
                    default: {
                        std::cout << "Selection is not valid." << std::endl;
                        SetState(WaitForAccountSelection); break;                        
                    }
                }
                std::cout << std::endl;
                break;
            }
            case WaitForActionSelection: {
                std::cout << "Please choose an option: " << std::endl;
                std::cout << "1. Check balance" << std::endl;
                std::cout << "2. Deposite" << std::endl;
                std::cout << "3. Withdraw" << std::endl;
                std::cout << "9. Return to account selection" << std::endl;
                std::cout << "0. Exit" << std::endl;
                std::cout << "Entered option:";                
                // std::cin >> input;
                unsigned int input = GetUserInput();
                switch (input) {
                    case 0: {
                        SetState(Exit); break;                        
                    }
                    case 1: {
                        SetState(WaitForCheckBalance); break;
                    }
                    case 2: {
                        SetState(WaitForDeposit); break;
                    }
                    case 3: {
                        SetState(WaitForWithdraw); break;
                    }
                    case 9: {
                        SetState(WaitForAccountSelection); break;
                    }
                    default: {
                        std::cout << "Selected option is not valid." << std::endl;
                        SetState(WaitForActionSelection); break;                        
                    }
                }
                std::cout << std::endl;
                break;
            }
            case WaitForCheckBalance: {
                std::cout << "Checking account balance... " << std::endl;
                CheckBalance();
                SetState(WaitForActionSelection);
                std::cout << std::endl;
                break;
            }
            case WaitForDeposit: {
                std::cout << "Amount to deposit: $";
                unsigned int deposit_amt = GetUserInput();
                Deposit(deposit_amt);
                SetState(WaitForActionSelection);
                std::cout << std::endl;
                break;
            }
            case WaitForWithdraw: {
                std::cout << "Amount to withdraw: $";
                unsigned int withdraw_amt = GetUserInput();
                Withdraw(withdraw_amt);
                SetState(WaitForActionSelection);
                std::cout << std::endl;
                break;
            }
            case Exit: {
                pin_verified_ = false;
                account_ptr_ = nullptr;
                SetState(WaitForCard);
                std::cout << "Exiting current account... Goodbye."<< std::endl;
                std::cout << std::endl;
                break;
            }
        }
    }
    return;
}

void ATMController::GetBankDatabase(const std::shared_ptr<Database> bank_database) {
    database_ptr = bank_database;
}


int ATMController::GetUserInput() {
    int input;
    std::cin >> input;
    while (true) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout <<"Please enter a non-negative number:";
            std::cin >> input;
        } else if (input < 0){
            std::cout <<"Please enter a non-negative number:";
            std::cin >> input;
        } else {
            break;
        }
    }
    return input;
}

bool ATMController::InsertCard(unsigned int card_number) {
    if(database_ptr == nullptr) {
        std::cout << "No bank database has been loaded yet." << std::endl;
        return false;
    }
    if(database_ptr->find(card_number) == database_ptr->end()){
        std::cout << "No bank account with card number of " << card_number << " can be found." << std::endl;
        return false;
    }
    account_ptr_ = database_ptr->at(card_number);
    return true;
}
    
bool ATMController::VerifyPin(unsigned int pin) {
    if(!account_ptr_) {
        std::cout << "Bank account does not exist." << std::endl;
        return false;
    }

    if(account_ptr_->VerifyPin(pin)) {
        pin_verified_ = true;
        std::cout << "Input pin is correct." << std::endl;
    } else {
        pin_verified_ = false;
        std::cout << "Input pin is incorrect." << std::endl;
    }
    return pin_verified_;
}

void ATMController::SelectAccount(BankAccount::AccountType type) {
    if(account_ptr_->SelectAccount(type)) {
        std::cout << "Current account: " << account_ptr_->GetCurrentAccountName() << std::endl;
        return;
    }
    std::cout << "Failed to select " << account_ptr_->GetCurrentAccountName() << std::endl;
    return;
}

void ATMController::CheckBalance() {
    if(!account_ptr_) {
        std::cout << "Account has not been selected yet." << std::endl;
        return;
    }
    if(!pin_verified_) {
        std::cout << "Pin has not been validated yet." << std::endl;
        return;
    }
    std::cout << "Current balance in " << account_ptr_->GetCurrentAccountName() << ": $" << account_ptr_->GetBalance() << std::endl;
    return;
}

void ATMController::Deposit(unsigned int amount) {
    if(!account_ptr_) {
        std::cout << "Account has not been selected yet." << std::endl;
        return;
    }
    if(!pin_verified_) {
        std::cout << "Pin has not been validated yet." << std::endl;
        return;
    }
    account_ptr_->Deposit(amount);
    std::cout << "Depositing $" << amount << " into " << account_ptr_->GetCurrentAccountName() << std::endl;
    CheckBalance();
}

int ATMController::Withdraw(unsigned int amount) {
    if(!account_ptr_) {
        std::cout << "Account has not been selected yet." << std::endl;
        return 0;
    }
    if(!pin_verified_) {
        std::cout << "Pin has not been validated yet." << std::endl;
        return 0;
    }
    if(amount > account_ptr_->GetBalance()) {
        std::cout << "Error: Withdraw amount is larger than current balance." << std::endl;
        CheckBalance();
        return 0;
    }
    int withdraw_amount = account_ptr_->WithDraw(amount);
    std::cout << "Withdrawing $" << amount << " from " << account_ptr_->GetCurrentAccountName() << std::endl;
    CheckBalance();
    return withdraw_amount;
}

void ATMController::SetState(const State s) {
    atm_state_ = s;
}
