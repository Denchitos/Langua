#pragma once
#include<string>
#include "init.h"
#include "classes.h"
#include<vector>
#include"frame.h"
#if 0
using Token = std::string;
using TokenSequence = std::vector<std::string>;
#endif

enum class TokenType
{
    Null,
    BinaryOperator,
    UnaryOperator,
    Number,
    StringValue,
    Variable,
    Function,
    Expression,
};

struct Token
{
    std::string value;
    TokenType type;

    Token(const std::string& v,TokenType t);
    void print() const;
};

class Lexer{
    std::vector<Token> result;
    std::string currentToken;
    TokenType currentType = TokenType::Null;
public:
    void update();
    std::vector<Token> tokenize(const std::string& s);
};

enum class ExprType
{
    Operation,
    UnarOperation,
    Number,
    Variable
};


class ExprElem
{
    ExprType type;
    ExprElem* parent = nullptr;
public:
    ExprElem(ExprType t);
    virtual ExprType getType();
    void setParent(ExprElem* p);
    ExprElem* getParent();
    virtual Object getValue(Frame* frame);
    virtual ~ExprElem();
    Object count(Frame * f);
};

Object convert(const std::string& s);

class ExprNumber : public ExprElem
{
    Object value;
public:
    using type = Object;
    ExprNumber(std::string& s);
    Object getValue(Frame* frame) override;
};

class ExprVariable : public ExprElem
{
    std::string value;
public:
    using type = Object;
    ExprVariable(std::string& s);
    Object getValue(Frame* frame) override;
    std::string getName();
};

struct ExprOperation : public ExprElem
{
    ExprElem* right = nullptr;
    ExprElem* left  = nullptr;
    std::string op;
public:
    void setLeft( ExprElem* el );

    void setRight( ExprElem* el );

    ExprElem* getRight( );

    ExprElem* getLeft( );

    Object getValue(Frame* frame) override;

    ExprOperation(const std::string& op);
    ~ExprOperation();
};

struct ExprUnarOperation : public ExprElem
{
    ExprElem* child = nullptr;
    std::string op;
public:
    void setChild( ExprElem* el );

    ExprElem* getChild( );

    Object getValue(Frame* frame) override;

    ExprUnarOperation(const std::string& op);
    ~ExprUnarOperation();
};

std::vector<std::string> tokenize_expr(const std::string& _expr);

class Expression
{
    Lexer l;
    std::vector<Token> nexpr;
public:
    Expression(const std::string& _expr);

    Expression(const std::vector<Token>& tokens,size_t i,size_t j);

    //void print();

    //bool isVar(bool mustFindOperator = false);
    
    size_t find_next_operator();

    void unParenth();
    size_t log();

    ExprElem* makeTree();
};