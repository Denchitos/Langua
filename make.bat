@echo off
g++ lang.cpp source/init.cpp source/expr.cpp source/classes.cpp source/executors.cpp source/frame.cpp source/interpreter.cpp -std=c++20 -o lang.exe