#pragma once
#include<string>
#include<memory>
#include"lang_exception.h"
template<typename T>
using up = std::unique_ptr<T>;

enum class DataType //After Adding a new type, don't forget add this DataType to TypeToStr map in classes.cpp
{
    Int,
    Double,
    Bool,
    String
};

extern const std::unordered_map<DataType,std::string> TypeToStr;

class BaseLangObject;

class LangInt;
class LangDouble;

using Object = std::unique_ptr<BaseLangObject>;

class LEBadOperator: public LangException
{
public:
    LEBadOperator(const char* op,DataType t1,DataType t2);
    LEBadOperator(const char* op,DataType t1);
};


class BaseLangObject
{
protected:
    DataType type;
public:
    BaseLangObject(DataType _type);
    virtual DataType getType() const;
    virtual Object sum(Object&);
    virtual Object multiply(Object&);
    virtual Object substract(Object&);
    virtual Object divide(Object&);
    virtual Object negative();
    virtual Object inverse();
    virtual Object less(Object&);
    virtual Object greater(Object&);
    virtual Object equal(Object&);
    virtual Object gr_or_eq(Object&);
    virtual Object ls_or_eq(Object&);
    virtual Object AND(Object&);
    virtual Object OR(Object&);

    virtual std::string toStdStr() const;
    virtual ~BaseLangObject() = default;
};

///LANGINT///LANGINT///LANGINT///LANGINT///LANGINT///

class LangInt : public BaseLangObject
{
    int value;
public:
    LangInt(int);
    DataType getType() const override;
    Object sum(Object&) override;
    Object multiply(Object&) override;
    Object divide(Object&) override;
    Object substract(Object&) override;
    Object negative() override;
    Object less(Object&);
    Object greater(Object&);
    Object equal(Object&);
    Object gr_or_eq(Object&);
    Object ls_or_eq(Object&);
    int getValue();
    //up<LangInt> toInt();
    //up<LangDouble> toDouble();

    std::string toStdStr() const override;
    

    //Object cast(DataType type);
};

Object sum(Object&,Object&);
Object substract(Object&,Object&);
Object divide(Object&,Object&);
Object multiply(Object&,Object&);
void assignment();


Object newInt(int);
Object newInt(const Object&);

///LANGDOUBLE///LANGDOUBLE///LANGDOUBLE///LANGDOUBLE///LANGDOUBLE///

class LangDouble : public BaseLangObject
{
    double value;
public:
    LangDouble(double);
    DataType getType() const override;
    Object sum(Object&) override;
    Object multiply(Object&) override;
    Object divide(Object&) override;
    Object substract(Object&) override;
    Object negative() override;
    Object less(Object&);
    Object greater(Object&);
    Object equal(Object&);
    Object gr_or_eq(Object&);
    Object ls_or_eq(Object&);
    double getValue();
    std::string toStdStr() const override;

    //up<LangInt> toInt();
    //up<LangDouble> toDouble();

    //Object cast(DataType type);
};

Object newDouble(double);
Object newDouble(const Object&);


///LANGBOOL///LANGBOOL///LANGBOOL///LANGBOOL///LANGBOOL///

class LangBool : public BaseLangObject
{
    bool value;
public:
    LangBool(bool);
    DataType getType() const override;
    Object inverse() override;
    bool getValue();
    
    Object AND(Object&);
    Object OR(Object&);

    std::string toStdStr() const override;

    //up<LangInt> toInt();
    //up<LangDouble> toDouble();

    //Object cast(DataType type);
};

Object newBool(bool);
Object newBool(const Object&);


////LANGSTRING////LANGSTRING////LANGSTRING////LANGSTRING////LANGSTRING////


class LangString : public BaseLangObject
{
    std::string value;
public:
    LangString(const std::string& s);
    std::string getValue();
    DataType getType() const;
    Object sum(Object&);
    Object multiply(Object&);
    std::string toStdStr() const;
};

Object newString(const Object& v);
Object newString(const std::string& v);

////HELPING////HELPING////HELPING////HELPING////HELPING////

Object newObject(const Object&);
Object newObject(const std::string& rValue);

Object copyVar(const std::string& name);
void setVar(const std::string& name,Object& val);
void consoleLogVars();