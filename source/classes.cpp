#include"../headers/init.h"
#include"../headers/classes.h"
#include <unordered_map>
#include<memory>
const std::unordered_map<DataType,std::string> TypeToStr
{
    { DataType::Int, "INT" },
    { DataType::Double, "DOUBLE" },
    { DataType::Bool, "BOOL" }
};

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

Object BaseLangObject::sum(Object&) { throw LangException{0,std::string("Operation [ SUM ] is n't defined for [")+TypeToStr.at(type)+"]"}; }
Object BaseLangObject::multiply(Object&) { throw LangException{0,std::string("Operation [ MULTIPLY ] is n't defined for [")+TypeToStr.at(type)+"]"}; }
Object BaseLangObject::substract(Object&) { throw LangException{0,std::string("Operation [ SUBSTRACT ] is n't defined for [")+TypeToStr.at(type)+"]"}; }
Object BaseLangObject::divide(Object&) { throw LangException{0,std::string("Operation [ DIVIDE ] is n't defined for [")+TypeToStr.at(type)+"]"}; }
Object BaseLangObject::negative() { throw LangException{0,std::string("Operation [ NEGATIVE ] is n't defined for [")+TypeToStr.at(type)+"]"}; }
Object BaseLangObject::inverse() { throw LangException{0,std::string("Operation [ INVERSE ] is n't defined for [")+TypeToStr.at(type)+"]"}; }

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
    throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
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
    throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
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
    throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
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
    throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
}

Object LangInt::negative()
{
    return newInt(value*-1);
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


LangDouble::LangDouble(double x):BaseLangObject(DataType::Int),value(x){}

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
    else throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
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
    else throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
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
    else throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
    
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
    else throw LangException{0,std::string("Wrong Types: (") + TypeToStr.at(getType()) + ") AND (" + TypeToStr.at(rhs->getType())+")"};
    return newDouble( value * v2 );
}

Object LangDouble::negative()
{
    return newDouble(-1*value);
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
    return newBool(value);
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
Object newBool(const Object& ins)
{
    bool x = static_cast<LangBool*>(ins.get())->getValue();
    return newBool(x);
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
    throw LangException{0,std::string("Unknown Data Type to create")};
}












/////VARS/////VARS/////VARS/////VARS/////VARS/////VARS////


static std::unordered_map<std::string,Object> vars;

void setVar(const std::string& name,Object& val)
{
    logger<<" Var Registered \'"<< name <<"\' "<< val;
    vars[name] = std::move(val);
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