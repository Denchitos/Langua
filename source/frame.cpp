#include<iostream>
#include"../headers/frame.h"
#include"../headers/init.h"
void Frame::setVar(const std::string& name,Object& val)
{
    //std::cout<<"[FRAME] Setted variable: "<<name<<" "<<(int)val->getType()<<" "<<val->toStdStr()<<"\n";
    frame_vars[name] = std::move(val);
}

Object Frame::copyVar(const std::string& name)
{
    //std::cout<<"[FRAME] copy var\n";
    if (frame_vars.contains(name))
    {
        //std::cout<<"[FRAME] var found\n";
        const auto& z = frame_vars[name];
        return newObject(z);
    }
    //std::cout<<"Var Doesn't found\n";
    throw LangException{ 0,std::string("Unknown variable \'") + name + "\'\n"};
};

bool Frame::hasVar(const std::string & var)
{
    return frame_vars.contains(var);
}

void Frame::consoleLogVars()
{
    std::cout << "[SYSTEM][VARIABLES] {\n";
    if ( frame_vars.size() != 0 )
    {
        for ( const auto& [name,ptr] : frame_vars )
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

std::unordered_map<std::string,Object>& Frame::getMap()
    {
        return frame_vars;
    }

Frame::~Frame()
{

}

ScopedFrame::ScopedFrame(ScopedFrame* parent): parent(parent)
{
    
}

void ScopedFrame::setVar(const std::string& name,Object& val)
{
    ScopedFrame* current = this;
    while(current)
    {
        auto it = current->getMap().find(name);
        if (it != current->getMap().end())
        {
            it->second = std::move(val);
            return;
        }
        current = current->parent;
    }
    frame_vars[name] = std::move(val);
}

Object ScopedFrame::copyVar(const std::string& name)
{
    //std::cout<<"TRY TO FIND\n";
    if (frame_vars.contains(name))
    {
        //std::cout<<"FOUND in this SCOPE\n";
        const auto& z = frame_vars[name];
        //std::cout<<"[FRAME 1]"<<(int)z->getType()<<" "<<z->toStdStr();
        return newObject(z);
    }
    else if (parent) return parent->copyVar(name);
    else throw LangException(0,std::string("Unknown Variable: ")+name);
}

void ScopedFrame::consoleLogVars()
{
    std::cout << "[SYSTEM][VARIABLES] {\n";
    if (parent)
    {
        std::cout<<"[PARENT]\n";
        parent->consoleLogVars();
    }
    if ( frame_vars.size() != 0 )
    {
        for ( const auto& [name,ptr] : frame_vars )
        {
            auto it = ptr->getType();
            std::cout<<"\t"<<name<<" ("<< TypeToStr.at(it) <<") = " << ptr->toStdStr()<<"\n";
        }
    }
    else
    {
        std::cout << "\tNO VARIABLES\n";
    }
    //std::cout << "}\n";
}

ScopedFrame* ScopedFrame::getParent()
{
    return parent;  
}



ScopedFrame::~ScopedFrame()
{

}


