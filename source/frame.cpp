#include<iostream>
#include"../headers/frame.h"
#include"../headers/init.h"
void Frame::setVar(const std::string& name,Object& val)
{
    std::cout<<"[FRAME] Setted variable: "<<name<<" "<<(int)val->getType()<<" "<<val->toStdStr()<<"\n";
    frame_vars[name] = std::move(val);
}

void Frame::setVar(const std::string& name,Object&& val)
{
    std::cout<<"[FRAME] Setted and Moved variable: "<<name<<" "<<(int)val->getType()<<" "<<val->toStdStr()<<"\n";
    frame_vars[name] = std::move(val);
}

Object Frame::copyVar(const std::string& name)
{
    std::cout<<"[FRAME] copy var\n";
    if (frame_vars.contains(name))
    {
        std::cout<<"[FRAME] var found\n";
        const auto& z = frame_vars[name];
        return newObject(z);
    }
    std::cout<<"Var Doesn't found\n";
    throw LangException{ 0,std::string("Unknown variable \'") + name + "\'\n"};
};

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

ScopedFrame::ScopedFrame(Frame* parent): parent(parent)
{
    
}

void ScopedFrame::setVar(const std::string& name,Object& val)
{
    std::cout<<"[SCOPED FRAME] Setted variable: "<<name<<" "<<(int)val->getType()<<" "<<val->toStdStr()<<"\n";
    frame_vars[name] = std::move(val);
}

void ScopedFrame::setVar(const std::string& name,Object&& val)
{
    std::cout<<"[FRAME] Setted and Moved variable: "<<name<<" "<<(int)val->getType()<<" "<<val->toStdStr()<<"\n";
    frame_vars[name] = std::move(val);
}

Object ScopedFrame::copyVar(const std::string& name)
{
    std::cout<<"TRY TO FIND\n";
    if (frame_vars.contains(name))
    {
        std::cout<<"FOUND in this SCOPE\n";
        const auto& z = frame_vars[name];
        std::cout<<"[FRAME 1]"<<(int)z->getType()<<" "<<z->toStdStr();
        return newObject(z);
    }
    std::cout<<"LEVEL UPPING\n";
    try{
        return parent->copyVar(name);
    }
    catch(LangException& e)
    {
        std::cout<<e.cause<<"\n";
    }
    catch(std::exception& e)
    {
        std::cout<<e.what()<<"\n";
    }
    std::cout<<"[Fawatafa]\n";
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
    std::cout << "}\n";
}

Frame* ScopedFrame::getParent()
{
    return parent;  
}



ScopedFrame::~ScopedFrame()
{
    for (auto & [name , var] : frame_vars)
    {
        if (parent->getMap().contains(name) && parent->getMap()[name]->getType() == var->getType() )
        {
            parent->setVar(name,newObject(var));
        }
    }
}


