cmake_minimum_required(VERSION 3.23.2)
project(BankSystem C)

set(CMAKE_C_STANDARD 23)

add_executable(BankSystem main.c dynamic_list.c dynamic_list.h account.c account.h)
