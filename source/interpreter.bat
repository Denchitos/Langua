@echo off
del int.exe
g++ lang.cpp executors.cpp frame.cpp classes.cpp init.cpp expr.cpp interpreter.cpp -std=c++20 -o int.exe
int.exe
