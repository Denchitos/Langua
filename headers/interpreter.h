#pragma once
#include<iostream>
#include<unordered_map>
#include<string>
#include<vector>
#include <algorithm>
#include<stack>
#include<memory>
#include "expr.h"

extern const size_t CodeLexerReserve;
extern const char SPACE;
enum class InstrType
{
    Expression,
    If,
    Else,
    Scope,
    While
};

void erase(std::string& str,char er);

struct InstrToken
{
    InstrType type;
    std::string token;
    InstrToken(const std::string & s,InstrType _t);
};

extern const std::unordered_map<std::string,InstrType> keywords;
class CodeLexer
{
    std::string current;
    InstrType currentType;
    std::vector<InstrToken> tokens;
public:
    CodeLexer();
    std::vector<std::string> separate(const std::string& text);
    void tokenize(const std::string& code);
    void update();
    std::vector<InstrToken> get();
};

class InstrNode;
class InstrNodeExpr;
class InstrNodeScope;
class InstrNodeIf;
class InstrNodeWhile;

struct AbstractTraveller
{
protected:
    InstrNode* current = nullptr;
    std::stack<InstrNode*> points;
public:
    virtual void start(InstrNode* n) = 0;
    virtual void visit(InstrNodeExpr* n) = 0;
    virtual void visit(InstrNodeScope* n) = 0;
    virtual void visit(InstrNodeIf* n) = 0;
    virtual void visit(InstrNodeWhile* n) = 0;
    virtual ~AbstractTraveller();
};

class InstrNode
{
protected:
    InstrNode* prev;
    InstrNode* next = nullptr;
    static size_t counter;
public:
    InstrNode(InstrNode* _parent = nullptr);
    void setNext(InstrNode* n);
    void setPrev(InstrNode* n);
    virtual void print();
    InstrNode* getPrev();
    InstrNode* getNext();
    virtual ~InstrNode();
    virtual InstrNode* accept(AbstractTraveller* tr) = 0;
};

class InstrStart : public InstrNode
{
public:
    InstrNode* accept(AbstractTraveller* tr);
};

class InstrNodeExpr:public InstrNode
{
    std::string expr;
    ExprElem* head;
public:
    InstrNodeExpr(const std::string& _e);
    std::string get();

    void print();
    Object count(Frame* f);
    InstrNode* accept(AbstractTraveller* tr);
    ~InstrNodeExpr();
};

class InstrNodeScope:public InstrNode
{
    InstrNode* scope;
public:
    InstrNodeScope(InstrNode* x);
    void setScope(InstrNode* x);
    InstrNode* getScope();
    ~InstrNodeScope();
    InstrNode* accept(AbstractTraveller* tr);
};

class InstrNodeIf:public InstrNode
{
    InstrNodeExpr* condition = nullptr;
    InstrNode* thenBlock = nullptr;
    InstrNode* elseBlock = nullptr;
public:
    InstrNodeIf();
    void setCondition(InstrNode* x);
    void setThen(InstrNode* x);
    void setElse(InstrNode* x);

    InstrNodeExpr* getCondition();
    InstrNode* getThen();
    InstrNode* getElse();

    ~InstrNodeIf();
    InstrNode* accept(AbstractTraveller* tr);
};

class InstrNodeWhile:public InstrNode
{
    InstrNodeExpr* condition = nullptr;
    InstrNode* scope = nullptr;
public:
    InstrNodeWhile();
    
    InstrNodeExpr* getCondition();
    void setCondition(InstrNode* n);
    InstrNode* getThen();
    void setThen(InstrNode* n);
    ~InstrNodeWhile();
    InstrNode* accept(AbstractTraveller* tr);
};

InstrNode* makeNode(const InstrToken& node);


class CodeParser
{
public:
    InstrNode* makeTree(std::vector<InstrToken> tokens);
};