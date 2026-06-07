#include "../headers/init.h"
#include "../headers/classes.h"
static bool debug = false;

Logger logger;

const std::unordered_map<std::string,short> operators_weigth{
    { "=", 0 }, { "-=", 0 }, { "+=", 0 }, { "*=", 0 }, { "/=", 0 },
    { "+" , 5 },{ "-" , 5 },
    {  "/", 6 },{  "*", 6 },
};

const std::unordered_set<std::string> unary_operators
{
    "-","!"
};

const std::unordered_set<std::string> binary_operators
{
    "=","-=","+=","*=","/=","+","-" ,"/","*"
};

const std::unordered_map<std::string,Object (BaseLangObject::*)(Object&)> operator_match
{
    { "+", &BaseLangObject::sum },
    { "-", &BaseLangObject::substract},
    { "/", &BaseLangObject::divide },
    { "*", &BaseLangObject::multiply}
};

const std::unordered_map<std::string,Object (BaseLangObject::*)()> unary_operator_match
{
    { "-", &BaseLangObject::negative },
    { "!", &BaseLangObject::inverse },
};

const std::unordered_map<std::string,std::string>  open_parantheses_match{ {"(",")"} };
const std::unordered_map<std::string,std::string> close_parantheses_match{ {")","("} };
#if 0
static const std::unordered_map<DataType,std::string> TypeToStr
{
    { DataType::Int, "INT" },
    { DataType::Double, "DOUBLE" },
    { DataType::Bool, "BOOL" }
};
#endif

const std::unordered_set<std::string> string_value{ "true" , "false"};

const std::string letters = ".abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::string numbers = "0123456789";
const std::string operator_symbols = "+-/*=!";

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


#if 0
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////

std::string BaseLangObject::toStdStr()const
{
    return "$MISSINGNO";
};

////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////


LangInt::LangInt(int x):value(x){}

DataType LangInt::getType() const
{
    return DataType::Int;
}

std::string LangInt::toStdStr() const
{
    return std::to_string(value);
}

Object LangInt::sum(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
    }
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return newInt( value+ v2);
}

Object LangInt::substract(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return newInt( value - v2);
}

Object LangInt::divide(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    if (v2==0) throw LangException{0,std::string("Division by Zero")};
    return newInt( value / v2);
}

void LangInt::negative()
{
    value = -1*value;
}


Object LangInt::multiply(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return newInt( value * v2);
}

int LangInt::getValue()
{
    return value;
}

Object newInt(int v)
{
    return std::make_unique<LangInt>(v);
}

Object newInt(const Object& ins)
{
    int x = static_cast<LangInt*>(ins.get())->getValue();
    return newInt(x);
}


////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////
////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////
////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////
////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////
////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////
////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////LANGDOUBLE////


LangDouble::LangDouble(double x):value(x){}

DataType LangDouble::getType() const
{
    return DataType::Double;
}

std::string LangDouble::toStdStr() const
{
    return std::to_string(value);
}

Object LangDouble::sum(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
    }
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return newDouble( value+ v2);
}

Object LangDouble::substract(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return newDouble( value - v2);
}

Object LangDouble::divide(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    if (v2==0) throw LangException{0,std::string("Division by Zero")};
    return newDouble( value / v2);
}

Object LangDouble::multiply(Object& rhs)
{
    if (this->getType() != rhs->getType())
    {
        throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(getType())+")"};
    }
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return newDouble( value * v2);
}

void LangDouble::negative()
{
    value = -1*value;
}

int LangDouble::getValue()
{
    return value;
}

Object newDouble(double v)
{
    return std::make_unique<LangDouble>(v);
}

Object newDouble(const Object& ins)
{
    int x = static_cast<LangDouble*>(ins.get())->getValue();
    return newDouble(x);
}


/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////
/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////
/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////
/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////
/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////HELPING/////

Object sum(Object& o1,Object& o2)
{
    return o1->sum(o2);
}

Object substract(Object& o1,Object& o2)
{
    return o1->substract(o2);
}

Object divide(Object& o1,Object& o2)
{
    return o1->divide(o2);
}

Object multiply(Object& o1,Object& o2)
{
    return o1->multiply(o2);
}

Object newObject(const Object& z)
{
    if (z->getType() == DataType::Int) return newInt(z);
    if (z->getType() == DataType::Double) return newDouble(z);
}

#endif

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