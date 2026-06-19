#pragma once
#include<string>
#include<iostream>
#include<iomanip>
#include<sstream>
#include<unordered_map>
#include<unordered_set>
#include"classes.h"
#include"lang_exception.h"
////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////

template<typename T>
concept Loggable = requires(std::ostream& os, T& instance)
{
    { os << instance} -> std::convertible_to<std::ostream&>;
};

void setDebugStream(std::ostream* out = &std::cout);
bool getDebug();
class Logger
{

    std::ostringstream oss;
    std::ostream* out = &std::cout;
public:
    Logger();
    
    template<Loggable T>
    Logger& operator<<(T&& mes)
    {
        if ( getDebug() && out)
        {
            oss << mes;
        }
        return *this;
    }
    
    void log();
    friend void setDebugStream( std::ostream* out);
};

bool isLetter(char c);
bool isDigit(char c);
bool isOperator(char c);

void setDebugMode(bool m = true);
extern Logger logger;
extern const std::unordered_set<char> nonVariable;
extern const std::unordered_map<std::string,short> operators_weigth;
extern const std::unordered_map<std::string,std::string>  open_parantheses_match;
extern const std::unordered_map<std::string,std::string> close_parantheses_match;
extern const std::unordered_map<std::string,Object (BaseLangObject::*)(Object&)> operator_match;
extern const std::unordered_map<std::string,Object (BaseLangObject::*)()> unary_operator_match;
extern const std::string letters;
extern const std::string numbers;
extern const std::string operator_symbols;
extern const std::unordered_map<std::string,DataType> string_value;
extern const std::unordered_set<std::string> unary_operators;
extern const std::unordered_set<std::string> binary_operators;
extern const std::unordered_set<std::string> all_operators;
bool canBeVar(std::string& s);