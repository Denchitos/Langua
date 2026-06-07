#pragma once
#include<string>
#include "init.h"
#include<vector>

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
    std::vector<Token> new_tokenize_expr(const std::string& s);
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
    virtual Object getValue();
    virtual ~ExprElem();
    Object count();
};

Object convert(const std::string& s);

class ExprNumber : public ExprElem
{
    Object value;
public:
    using type = Object;
    ExprNumber(std::string& s);
    Object getValue() override;
};

class ExprVariable : public ExprElem
{
    std::string value;
public:
    using type = Object;
    ExprVariable(std::string& s);
    Object getValue() override;
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

    Object getValue();

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

    Object getValue();

    ExprUnarOperation(const std::string& op);
    ~ExprUnarOperation();
};

std::vector<std::string> tokenize_expr(const std::string& _expr);

class Expression
{
    std::vector<std::string> nexpr;
public:
    Expression(const std::string& _expr);

    Expression(const std::vector<std::string>& tokens,int i,int j);

    void print();

    bool isVar(bool mustFindOperator = false);
    
    size_t find_next_operator();

    void unParanth();
    size_t log();

    ExprElem* makeTree();
};