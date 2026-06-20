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
    f = new ScopedFrame;
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
    if (n->get() == "PRINT_FRAME"){
        std::cout<<"[PRINT FRAME MACRO]";
        f->consoleLogVars();
        return;
    }
    else if (n->get() == "PRINT"){
        auto instr = n->getNext();
        if (instr==nullptr) throw LangException{0,std::string("There's no variable to print!")};
        else if ( auto d = dynamic_cast<InstrNodeExpr*>(instr) )
        {
            auto head = d->getHead();
            std::cout<<head->count(f)->toStdStr();
        }
        else
        {
            std::cout<<"\n";
            instr->print();
            throw LangException{0,std::string("<-- This isn't an expression;")};
        }
    }
    else if (n->get() == "INPUT"){
            while (true)
            {
                std::string input;
                std::getline(std::cin,input);
                Object a = nullptr;
                try{
                    a = newObject(input);
                }
                catch (LangException& e)
                {
                    std::cout<<e.getCause()<<"\nTry input again\n";
                    continue;
                }
                f->setVar("_input",a);
                break;
            }
    }
    else if (n->get()=="PAUSE")
    {
        system("pause");
    }
    else n->count(f);
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
    auto then = n->getThen();
    auto els = n->getElse();
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

void ExecutorTraveller::visit(InstrNodeWhile* n)
{
    auto cond = n->getCondition();
    auto then = n->getThen();
    while (cond->count(f)->toStdStr() == "true")
    {
        //std::cout<<"[IN WHILE]\n";
        start(then);
    }
}


ExecutorTraveller::~ExecutorTraveller()
{
    if (f) delete f;
}