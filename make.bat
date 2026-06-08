@echo off
del lang.exe
g++ lang.cpp source/init.cpp source/expr.cpp source/classes.cpp -std=c++20 -o lang.exe
lang.exe