#include"../headers/init.h"
#include"../headers/classes.h"
#include <unordered_map>
#include<memory>
const std::unordered_map<DataType,std::string> TypeToStr
{
    { DataType::Int, "INT" },
    { DataType::Double, "DOUBLE" },
    { DataType::Bool, "BOOL" },
    { DataType::String, "STRING" }
};

LEBadOperator::LEBadOperator(const char* op,DataType t1,DataType t2)
{
    cause = std::string("Binary Operation [") + op + "] is n't defined for ["+TypeToStr.at(t1)+"] AND [" + TypeToStr.at(t2) + "]";
}

LEBadOperator::LEBadOperator(const char* op,DataType t1)
{
    cause = std::string("Unary Operation [") + op + "] is n't defined for ["+TypeToStr.at(t1)+"]";
}


////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////
////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////BASELANGOBJECT////////BASELANGOBJECT////////BASELANGOBJECT////

BaseLangObject::BaseLangObject(DataType _type): type(_type){};

DataType BaseLangObject::getType() const
{
    return type;
}

std::string BaseLangObject::toStdStr()const
{
    return "$MISSINGNO";
};

Object BaseLangObject::sum(Object& rhs)       { throw LEBadOperator("SUM",type,rhs->getType() ); }
Object BaseLangObject::multiply(Object& rhs)  { throw LEBadOperator("MULTIPLY",type,rhs->getType() ); }
Object BaseLangObject::substract(Object& rhs) { throw LEBadOperator("SUB",type,rhs->getType() ); }
Object BaseLangObject::divide(Object& rhs)    { throw LEBadOperator("DIVIDE",type,rhs->getType() ); }
Object BaseLangObject::negative()             { throw LEBadOperator("NEGATIVE",type); }
Object BaseLangObject::inverse()              { throw LEBadOperator("INVERSE",type); }
Object BaseLangObject::less(Object& rhs)      { throw LEBadOperator("LESS",type,rhs->getType() ); }
Object BaseLangObject::greater(Object& rhs)   { throw LEBadOperator("GREATER",type,rhs->getType() ); }
Object BaseLangObject::equal(Object& rhs)     { throw LEBadOperator("EQUAL",type,rhs->getType() ); }
Object BaseLangObject::ls_or_eq(Object& rhs)      { throw LEBadOperator("<=",type,rhs->getType() ); }
Object BaseLangObject::gr_or_eq(Object& rhs)      { throw LEBadOperator(">=",type,rhs->getType() ); }
Object BaseLangObject::AND(Object& rhs)       { throw LEBadOperator("AND",type,rhs->getType() ); }
Object BaseLangObject::OR(Object& rhs)        { throw LEBadOperator("OR",type,rhs->getType() ); }

//up<LangInt> BaseLangObject::toInt() { throw LangException{0,std::string("No Conversation from [") +TypeToStr.at(type) + "] to [INT]" }; }
//up<LangDouble> BaseLangObject::toDouble() { throw LangException{0,std::string("No Conversation from [") +TypeToStr.at(type) + "] to [DOUBLE]" }; }

////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////
////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////LANGINT////


LangInt::LangInt(int x):BaseLangObject(DataType::Int),value(x){}

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
    if (rhs->getType() == DataType::Int)
    {
        int v2 = static_cast<LangInt*>(rhs.get())->value;
        return newInt( value+ v2);
    }
    if (rhs->getType() == DataType::Double)
    {
        double v2 = static_cast<LangDouble*>(rhs.get())->getValue();
        return newDouble( value+ v2);
    }
    throw LEBadOperator("SUM",type,rhs->getType() );
}

Object LangInt::substract(Object& rhs)
{
    if (rhs->getType() == DataType::Int)
    {
        int v2 = static_cast<LangInt*>(rhs.get())->value;
        return newInt( value -v2);
    }
    if (rhs->getType() == DataType::Double)
    {
        double v2 = static_cast<LangDouble*>(rhs.get())->getValue();
        return newDouble( value - v2);
    }
    throw LEBadOperator("SUB",type,rhs->getType() );
}

Object LangInt::divide(Object& rhs)
{
    if (rhs->getType() == DataType::Int)
    {
        int v2 = static_cast<LangInt*>(rhs.get())->value;
        if (v2==0) throw LangException{0,std::string("Division by Zero")};
        return newInt( value / v2);
    }
    if (rhs->getType() == DataType::Double)
    {
        double v2 = static_cast<LangDouble*>(rhs.get())->getValue();
        if (v2==0.0) throw LangException{0,std::string("Division by Zero")};
        return newDouble( value / v2);
    }
    throw LEBadOperator("DIVIDE",type,rhs->getType() );;
}

Object LangInt::multiply(Object& rhs)
{
    if (rhs->getType() == DataType::Int)
    {
        int v2 = static_cast<LangInt*>(rhs.get())->value;
        return newInt( value * v2);
    }
    if (rhs->getType() == DataType::Double)
    {
        double v2 = static_cast<LangDouble*>(rhs.get())->getValue();
        return newDouble( value * v2);
    }
    if (rhs->getType() == DataType::String)
    {
        auto t = newInt(value);
        return rhs->multiply( t );
    }
    throw LEBadOperator("MULTIPLY",type,rhs->getType() );
}

Object LangInt::negative()
{
    return newInt(value*-1);
}


Object LangInt::less(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("LESS",type,rhs->getType() );;
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return value<v2 ? newBool(true) : newBool(false);
}

Object LangInt::greater(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("GREATER",type,rhs->getType() );;
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return value>v2 ? newBool(true) : newBool(false);
}

Object LangInt::equal(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("EQUAL",type,rhs->getType() );;
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return value==v2 ? newBool(true) : newBool(false);
}

Object LangInt::gr_or_eq(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator(">=",type,rhs->getType() );;
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return value>=v2 ? newBool(true) : newBool(false);
}

Object LangInt::ls_or_eq(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("<=",type,rhs->getType() );;
    int v2 = static_cast<LangInt*>(rhs.get())->value;
    return value<=v2 ? newBool(true) : newBool(false);
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


LangDouble::LangDouble(double x):BaseLangObject(DataType::Double),value(x){}

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
    double v2;
    if (rhs->getType()==DataType::Double)
    {
        v2 = static_cast<LangDouble*>(rhs.get())->value;
    }
    else if ( rhs->getType()==DataType::Int)
    {
        v2 = static_cast<LangInt*>(rhs.get())->getValue();
    }
    else throw LEBadOperator("SUM",type,rhs->getType() );
    return newDouble( value+ v2 );
}

Object LangDouble::substract(Object& rhs)
{
    double v2;
    if (rhs->getType()==DataType::Double)
    {
        v2 = static_cast<LangDouble*>(rhs.get())->value;
    }
    else if ( rhs->getType()==DataType::Int)
    {
        v2 = static_cast<LangInt*>(rhs.get())->getValue();
    }
    else throw LEBadOperator("SUB",type,rhs->getType() );;
    return newDouble( value - v2 );
}

Object LangDouble::divide(Object& rhs)
{
    double v2;
    if (rhs->getType()==DataType::Double)
    {
        v2 = static_cast<LangDouble*>(rhs.get())->value;
    }
    else if ( rhs->getType()==DataType::Int)
    {
        v2 = static_cast<LangInt*>(rhs.get())->getValue();
    }
    else throw LEBadOperator("DIVIDE",type,rhs->getType() );
    
    if (v2==0) throw LangException{0,std::string("Division by Zero")};

    return newDouble( value / v2 );
}

Object LangDouble::multiply(Object& rhs)
{
    double v2;
    if (rhs->getType()==DataType::Double)
    {
        v2 = static_cast<LangDouble*>(rhs.get())->value;
    }
    else if ( rhs->getType()==DataType::Int)
    {
        v2 = static_cast<LangInt*>(rhs.get())->getValue();
    }
    else throw LEBadOperator("MULTIPLY",type,rhs->getType() );
    return newDouble( value * v2 );
}

Object LangDouble::negative()
{
    return newDouble(-1*value);
}

Object LangDouble::less(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("LESS",type,rhs->getType() );;
    int v2 = static_cast<LangDouble*>(rhs.get())->value;
    return value<v2 ? newBool(true) : newBool(false);
}

Object LangDouble::greater(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("LESS",type,rhs->getType() );;
    int v2 = static_cast<LangDouble*>(rhs.get())->value;
    return value>v2 ? newBool(true) : newBool(false);
}

Object LangDouble::equal(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("LESS",type,rhs->getType() );;
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return value==v2 ? newBool(true) : newBool(false);
}

Object LangDouble::gr_or_eq(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator(">=",type,rhs->getType() );;
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return value>=v2 ? newBool(true) : newBool(false);
}

Object LangDouble::ls_or_eq(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("<=",type,rhs->getType() );;
    double v2 = static_cast<LangDouble*>(rhs.get())->value;
    return value<=v2 ? newBool(true) : newBool(false);
}

double LangDouble::getValue()
{
    return value;
}

Object newDouble(double v)
{
    return std::make_unique<LangDouble>(v);
}

Object newDouble(const Object& ins)
{
    double x = static_cast<LangDouble*>(ins.get())->getValue();
    return newDouble(x);
}



////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////LANGBOOL////
LangBool::LangBool(bool x): BaseLangObject(DataType::Bool), value(x){}
DataType LangBool::getType() const
{
    return DataType::Bool;
}
Object LangBool::inverse()
{
    return newBool(!value);
};

Object LangBool::AND(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("AND",type,rhs->getType() );;
    auto v = static_cast<LangBool*>(rhs.get())->value;
    return newBool(value&&v);
};

Object LangBool::OR(Object& rhs)
{
    if ( rhs->getType() != getType() ) throw LEBadOperator("OR",type,rhs->getType() );;
    auto v = static_cast<LangBool*>(rhs.get())->value;
    return newBool(value||v);
};

bool LangBool::getValue()
{
    return value;
};
std::string LangBool::toStdStr() const
{
    if (value) return "true";
    return "false";
};

Object newBool(bool v)
{
    return std::make_unique<LangBool>(v);
}
Object newBool(const std::string& v)
{
    if (v=="true") return std::make_unique<LangBool>(true);
    if (v=="false") return std::make_unique<LangBool>(false);
    throw LangException{0,std::string("Data type [BOOL] can't has rvalue: ")+v};
}
Object newBool(const Object& ins)
{
    bool x = static_cast<LangBool*>(ins.get())->getValue();
    return newBool(x);
}


/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////
/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////
/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////
/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////
/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////LANGSTRING/////

static std::string replaceAll(std::string str,const std::string& from,const std::string& to)
{
    if (from.empty()) return str;
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

LangString::LangString(const std::string& s): BaseLangObject(DataType::String)
{
    auto str = s;
    if ( str.find("\\n") != std::string::npos ) str = replaceAll(str,"\\n","\n");
    if ( str.find("\\t") != std::string::npos ) str = replaceAll(str,"\\t","\t");
    value = str;
}
DataType LangString::getType() const
{
    return DataType::String;
}
std::string LangString::getValue()
{
    return value;
}
Object LangString::sum(Object& rhs)
{
    if (rhs->getType()!=DataType::String)
    {
        throw LEBadOperator( "SUM",type,rhs->getType() );
    }
    std::string x = static_cast<LangString*>(rhs.get())->getValue();
    return newString(value+x);
}
Object LangString::multiply(Object& rhs)
{
    if (rhs->getType()!=DataType::Int)
    {
        throw LEBadOperator( "MULTIPLY",type,rhs->getType() );
    }
    int x = static_cast<LangInt*>(rhs.get())->getValue();
    std::string s;
    for (int i =0;i<x;i++)
    {
        s+=value;
    }
    return newString(s);
}
std::string LangString::toStdStr() const
{
    return value;
}

Object newString(const std::string& v)
{
    return std::make_unique<LangString>(v);
}

Object newString(const Object& v)
{
    std::string x = static_cast<LangString*>(v.get())->getValue();
    return newString(x);
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
    if (z->getType() == DataType::Bool) return newBool(z);
    if (z->getType() == DataType::String) return newString(z);
    throw LangException{0,std::string("Unknown Data Type to create")};
}


Object newObject(const std::string& rValue)
{
    size_t len = rValue.length();
    char c = rValue[0];
    if (isDigit(c) || c=='-')
    {
        return (rValue.find('.')==std::string::npos) ? newInt( stoi(rValue) ) : newDouble(stod(rValue));
    }

    if (c=='\"')
    {
        return newString(rValue.substr(1,len-2));
    }

    DataType type;
    if ( string_value.contains(rValue) ) type = string_value.at(rValue); 

    if (type==DataType::Bool) return newBool(rValue);

    throw LangException{0,std::string("Can't deduce the type by rvalue: ")+rValue};
}









/////VARS/////VARS/////VARS/////VARS/////VARS/////VARS////


static std::unordered_map<std::string,Object> vars;

void setVar(const std::string& name,Object& val)
{
    //std::cout<<" Var Registered \'"<< name <<"\' "<<TypeToStr.at(val->getType())<<" "<< val->toStdStr()<<"\n";
    vars[name] = std::move(val);
    //std::cout<<"\t"<<name<<" ("<< TypeToStr.at(vars[name]->getType()) <<") = " << vars[name]->toStdStr()<<"\n";
    logger.log();
}

void consoleLogVars()
{
    std::cout << "[SYSTEM][VARIABLES] {\n";
    if ( vars.size() != 0 )
    {
        for ( const auto& [name,ptr] : vars )
        {
            auto it = ptr->getType();
            std::cout<<"\t"<<name<<" ("<< TypeToStr.at(it) <<") = " << ptr->toStdStr()<<"\n";
        }
    }
    else
    {
        std::cout << "\tNO VARIABLES\n";
    }
    std::cout << "}\n";
}

Object copyVar(const std::string& name)
{
    if (vars.contains(name))
    {
        const auto& z = vars[name];
        //std::cout<<name<<" FIND VAR ("<< TypeToStr.at(z->getType()) <<") = " << z->toStdStr()<<"\n";
        return newObject(z);
    }
    throw LangException{ 0,std::string("Unknown variable \'") + name + "\'\n"};
};