cmake_minimum_required(VERSION 2.8)

project(ATM_Controller)

add_library(lib_atm BankAccount.cpp ATMController.cpp)

add_executable(main main.cpp)
target_link_libraries(main lib_atm)