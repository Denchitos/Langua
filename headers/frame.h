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
    virtual void setVar(const std::string& name,Object&& val);
    virtual Object copyVar(const std::string& name);
    virtual void consoleLogVars();

    virtual ~Frame();
};

class ScopedFrame final: public Frame
{
    Frame* parent;
public:
    ScopedFrame(Frame* parent);
    Object copyVar(const std::string& name);
    void setVar(const std::string& name,Object& val);
    void setVar(const std::string& name,Object&& val);
    void consoleLogVars();
    Frame* getParent();
    ~ScopedFrame();
};