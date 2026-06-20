#pragma once
#include<unordered_map>
#include<string>
#include"classes.h"

class Frame
{
protected:
    std::unordered_map<std::string,Object> frame_vars;
public:
    virtual std::unordered_map<std::string,Object>& getMap();
    virtual void setVar(const std::string& name,Object& val);
    virtual Object copyVar(const std::string& name);
    virtual bool hasVar(const std::string & var);
    virtual void consoleLogVars();
    virtual ~Frame() = 0;
};

class ScopedFrame final: public Frame
{
    ScopedFrame* parent;
public:
    ScopedFrame(ScopedFrame* parent = nullptr);
    Object copyVar(const std::string& name);
    void setVar(const std::string& name,Object& val);
    void consoleLogVars();
    ScopedFrame* getParent();
    ~ScopedFrame();
};