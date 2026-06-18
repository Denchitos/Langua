#include"interpreter.h"

struct PrintTraveller final: public AbstractTraveller
{
    std::string pref;
    virtual void start(InstrNode* n);
    virtual void visit(InstrNodeExpr* n);
    virtual void visit(InstrNodeScope* n);
    virtual void visit(InstrNodeIf* n);
    virtual void visit(InstrNodeWhile* n);
};



struct ExecutorTraveller final: public AbstractTraveller
{
private:
    Frame* f;
public:
    ExecutorTraveller();
    ExecutorTraveller(ExecutorTraveller&) = delete;
    virtual void start(InstrNode* n);
    virtual void evaluate();
    virtual void visit(InstrNodeExpr* n);
    virtual void visit(InstrNodeScope* n);
    virtual void visit(InstrNodeIf* n);
    virtual void visit(InstrNodeWhile* n);
    ~ExecutorTraveller();
};