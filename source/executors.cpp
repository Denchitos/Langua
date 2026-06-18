#include"../headers/executors.h"

void PrintTraveller::start(InstrNode* n)
{
    InstrNode* current = n;
    while (current)
    {
        current = current->accept(this);
    }
}
void PrintTraveller::visit(InstrNodeExpr* n)
{
    std::cout<<pref<<n->get()<<"\n";
}
void PrintTraveller::visit(InstrNodeScope* n)
{
    std::cout<<pref<<"Scope\n";
    auto tmp = pref;
    pref+="|-";
    start(n->getScope());
    std::swap(pref,tmp);
}
void PrintTraveller::visit(InstrNodeIf* n)
{
    std::cout<<pref<<"IF\n";
    auto tmp = pref;
    pref+="|-";
    std::cout<<pref<<"[CONDITION]\n";
    start(n->getCondition());
    std::cout<<pref<<"[THEN BLOCK]\n";
    start(n->getThen());
    
    if (n->getElse())
    {
        std::cout<<pref<<"[ELSE BLOCK]\n";
        start(n->getElse());
    }
    std::swap(pref,tmp);
}
void PrintTraveller::visit(InstrNodeWhile* n)
{
    std::cout<<pref<<"WHILE\n";
    auto tmp = pref;
    pref+="|-";
    std::cout<<pref<<"[CONDITION]\n";
    start(n->getCondition());
    std::cout<<pref<<"[THEN BLOCK]\n";
    start(n->getThen());
    std::swap(pref,tmp);
}

ExecutorTraveller::ExecutorTraveller()
{
    f = new Frame;
}

void ExecutorTraveller::start(InstrNode* n)
{
    InstrNode* current = n;
    while (current)
    {
        current = current->accept(this);
    }
}
void ExecutorTraveller::evaluate()
{

}
void ExecutorTraveller::visit(InstrNodeExpr* n)
{
    if (n->get() == "PRINT"){
        std::cout<<"[PRINT MACRO]";
        f->consoleLogVars();
        return;
    }
    else if (n->get() == "INPUT"){
        std::cout<<"Input: ";
        std::string input;
        std::getline(std::cin,input);
        auto a = newObject(input);
        f->setVar("_input",a);
    }
    else if (n->get()=="PAUSE")
    {
        system("pause");
    }
    else try{
        n->count(f);
    }
    catch(LangException& e)
    {
        std::cout<<e.cause<<'\n';
    }
}
void ExecutorTraveller::visit(InstrNodeScope* n)
{
    auto upf = new ScopedFrame(f);
    //std::cout<<"[PRE SCOPE]";
    //f->consoleLogVars();
    f = upf;
    start(n->getScope());
    //std::cout<<"[PRE NEW SCOPE DELETION]";
    //f->consoleLogVars();
    f = upf->getParent();
    delete upf;
    //f->consoleLogVars();

}
void ExecutorTraveller::visit(InstrNodeIf* n)
{
    auto cond = n->getCondition();
    auto then = n->getThen();
    auto els = n->getElse();
    try{
        auto t = n->getCondition()->count(f);
        if (t->toStdStr() == "true")
        {
            start(then);
        }
        else if (els)
        {
            start(els);
        }
    }
    catch(LangException& e)
    {
        std::cout<<"ERROR";
        std::cout<<e.cause<<'\n';
    }
    catch(...)
    {
        std::cout<<"UNKNOWN ERROR";
    }
}

void ExecutorTraveller::visit(InstrNodeWhile* n)
{
    auto cond = n->getCondition();
    auto then = n->getThen();
    try{
        while (cond->count(f)->toStdStr() == "true")
        {
            //std::cout<<"[IN WHILE]\n";
            start(then);
        }
    }
    catch(LangException& e)
    {
        std::cout<<e.cause<<'\n';
    }
    catch(...)
    {
        std::cout<<"[UNKNOWN ERROR]"<<'\n';
    }
}


ExecutorTraveller::~ExecutorTraveller()
{
    if (f) delete f;
}