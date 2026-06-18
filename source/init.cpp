#include "../headers/init.h"
#include "../headers/classes.h"
static bool debug = false;

Logger logger;

const std::unordered_map<std::string,short> operators_weigth{
    { "=", 0 }, { "-=", 0 }, { "+=", 0 }, { "*=", 0 }, { "/=", 0 },
    {"<", 3},
    { "+" , 5 },{ "-" , 5 },
    {  "/", 6 },{  "*", 6 },
};

const std::unordered_set<std::string> unary_operators
{
    "-","!"
};

const std::unordered_set<std::string> binary_operators
{
    "=","-=","+=","*=","/=","+","-" ,"/","*","<"
};

const std::unordered_map<std::string,Object (BaseLangObject::*)(Object&)> operator_match
{
    { "+", &BaseLangObject::sum },
    { "-", &BaseLangObject::substract},
    { "/", &BaseLangObject::divide },
    { "*", &BaseLangObject::multiply},
    { "<", &BaseLangObject::less}
};

const std::unordered_map<std::string,Object (BaseLangObject::*)()> unary_operator_match
{
    { "-", &BaseLangObject::negative },
    { "!", &BaseLangObject::inverse },
};

const std::unordered_map<std::string,std::string>  open_parantheses_match{ {"(",")"},{"[","]"} };
const std::unordered_map<std::string,std::string> close_parantheses_match{ {")","("},{"]","["} };

const std::unordered_map<std::string,DataType> string_value
{
    { "true" , DataType::Bool},
    { "false", DataType::Bool}
};
const std::string letters = "$.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::string numbers = "0123456789";
const std::string operator_symbols = "<+-/*=!";

bool canBeVar(std::string& s)
{
    logger<<"CanBeVar ";
    //logger << letters.find(s[0]);
    logger.log();
    if (letters.find(s[0]) == std::string::npos) return false;
    logger<<"First Letter Okay";
    logger.log();
    for (size_t i=1;i<s.length();i++)
    {
        if ( !letters.find(s[i]) || !numbers.find(s[i]) ) return false;
    }
    return true;
}

bool isLetter(char c)
{
    return  letters.find(c) != std::string::npos; 
}

bool isDigit(char c)
{
    return  numbers.find(c) != std::string::npos; 
}

bool isOperator(char c)
{
    return  operator_symbols.find(c) != std::string::npos;
}

////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER
////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER
////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER
////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER
////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER////LOGGER

Logger::Logger(){}

void Logger::log()
{
    if (getDebug() && out) 
    {
        *out << "[LOGGER] " << oss.str() << std::endl;
        oss.str("");
        oss.clear(); 
        out->clear();
        system("pause"); 
    }
}

void setDebugStream(std::ostream* out)
{
    logger.out = out;
}

bool getDebug()
{
    return debug;
}

void setDebugMode(bool m)
{
    std::cout<<"[DEBUGMODE]\n";
    debug = m;
}