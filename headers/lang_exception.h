#pragma once
#include<string>
//////////////////////////////////////////
//////////DEFINED IN INIT.CPP/////////////
//////////////////////////////////////////

class LangException
{
protected:
    std::string cause;
public:
    LangException() = default;
    LangException(int,const std::string&);
    virtual std::string getCause() const;
};