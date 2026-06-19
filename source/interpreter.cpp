#include<iostream>
#include<unordered_map>
#include<string>
#include<vector>
#include <algorithm>
#include<fstream>
#include<stack>
#include<memory>
#include"../headers/interpreter.h"

const size_t CodeLexerReserve = 100;
const char SPACE = ' ';

void erase(std::string& str,char er)
{
    std::erase_if(str,[er](auto c){return c==er ; });
}

InstrToken::InstrToken(const std::string & s,InstrType _t): token(s),type(_t){}


const std::unordered_map<std::string,InstrType>keywords
{
    {"if",InstrType::If},
    {"else",InstrType::Else},
    {"while",InstrType::While}
};

CodeLexer::CodeLexer()
{
    tokens.reserve(CodeLexerReserve);
};
std::vector<std::string> separate(const std::string& text)
{
    size_t len = text.length();
    std::vector<std::string> result;
    std::string cur;
    size_t parenCounter = 0;
    size_t i = 0;
    while (i<len)
    {
        auto c = text[i];
        if (c=='{') parenCounter++;
        else if  (c=='}') parenCounter--;
        else if (  c == ';' && parenCounter>0 )
        {
            result.push_back(cur);
            cur.clear();
            ++i;
            while(i<len && (text[i] == ';' || text[i]==SPACE)) ++i;
            continue;
        }
        else if (parenCounter>0)
        {
            cur+=c;
            ++i;
            continue;
        }
        cur+=c;
        ++i;
    }
    if (!cur.empty()) result.push_back(cur);
    return result;
}
void CodeLexer::tokenize(const std::string& code)
{
    size_t i = 0, len = code.length();
    while (i<len)
    {
        if (code[i]==SPACE) { ++i;continue; }
        else if ( code[i] == '{' )
        {
            size_t parenCounter = 1;
            current+=code[i++];
            while( i<len && parenCounter>0)
            {
                if ( code[i]=='{' ) parenCounter++;
                if ( code[i]=='}' ) parenCounter--;
                current+=code[i++];
            }
            currentType = InstrType::Scope;
            update();
            continue;
        }
        else if ( code[i] == ';' )
        {
            i++;
            continue;
        }
        else
        {
            current+=code[i];
            i++;
            while( i<len && code[i] != '{' && code[i]!=';')
            {
                //if (code[i]==SPACE) { ++i;continue;} 
                current+=code[i++];
                if ( keywords.contains(current) && i<len && !isalpha(code[i]) ) break;
                else if ( keywords.contains(current) && i>=len  ) break;
            }
            if (keywords.contains(current)) currentType = keywords.at(current);
            else currentType = InstrType::Expression;
            update();
            continue;
        }
    }
}
void CodeLexer::update()
{
    tokens.emplace_back(current,currentType);
    current.clear();
}
std::vector<InstrToken> CodeLexer::get()
{
    return tokens;
}

InstrNode::InstrNode(InstrNode* _parent) : prev(_parent){ counter++; };
void InstrNode::setNext(InstrNode* n)
{
    next = n;
    if (n) n->prev = this;
}
void InstrNode::setPrev(InstrNode* n)
{
    prev = n;
    if (n) n->next = this;
}
void InstrNode::print(){};
InstrNode* InstrNode::getPrev()
{
    return prev;
}
InstrNode* InstrNode::getNext()
{
    return next;
}
//virtual void evaluate() = 0;

InstrNode* InstrStart::accept(AbstractTraveller* tr)
{
    return next;
}

InstrNode::~InstrNode()
{
    if (next) delete next;
    counter--;
    std::cout<<"[COUNTER] "<<counter<<"\n";
}
size_t InstrNode::counter = 0;


InstrNodeExpr::InstrNodeExpr(const std::string& _e): expr(_e)
{
    //std::cout<<"New eXpression\n";
    Expression tree(_e);
    head = tree.makeTree();
}
std::string InstrNodeExpr::get()
{
    return expr;
}

ExprElem* InstrNodeExpr::getHead()
{
    return head;
}

void InstrNodeExpr::print()
{
    std::cout<<expr;
}

Object InstrNodeExpr::count(Frame* f)
{
    auto x = head->count(f);
    return x;
}

InstrNode* InstrNodeExpr::accept(AbstractTraveller* tr)
{
    tr->visit(this);
    return next;
}
InstrNodeExpr::~InstrNodeExpr(){
    if (head) delete head;
    std::cout<<"Deleted Expression\n";
}


InstrNodeScope::InstrNodeScope(InstrNode* x)
{
    scope = x;
}
void InstrNodeScope::setScope(InstrNode* x)
{
    scope = x;
}
InstrNode* InstrNodeScope::getScope()
{
    return scope;
}
InstrNodeScope::~InstrNodeScope()
{
    std::cout<<"[Scope Deletion\n";
    if (scope) delete scope;
    std::cout<<"]\n";
}
InstrNode* InstrNodeScope::accept(AbstractTraveller* tr)
{
    tr->visit(this);
    return next;
}

InstrNodeIf::InstrNodeIf(  ){}

void InstrNodeIf::setCondition(InstrNode* x)
{
    condition = static_cast<InstrNodeExpr*>(x);
}
void InstrNodeIf::setThen(InstrNode* x)
{
    thenBlock = x;
}
void InstrNodeIf::setElse(InstrNode* x)
{
    elseBlock = x;
}

InstrNodeExpr* InstrNodeIf::getCondition(){ return condition; }
InstrNode* InstrNodeIf::getThen(){ return thenBlock; }
InstrNode* InstrNodeIf::getElse(){ return elseBlock; }

InstrNodeIf::~InstrNodeIf()
{
    if (condition) delete condition;
    if (thenBlock) delete thenBlock;
    if (elseBlock) delete elseBlock;
}

InstrNode* InstrNodeIf::accept(AbstractTraveller* tr)
{
    tr->visit(this);
    return next;
}


InstrNodeWhile::InstrNodeWhile() = default;

InstrNodeExpr* InstrNodeWhile::getCondition()
{
    return condition;
}
void InstrNodeWhile::setCondition(InstrNode* n)
{
    auto t = static_cast<InstrNodeExpr*>(n);
    condition = t;
}

InstrNode* InstrNodeWhile::getThen()
{
    return scope;
}
void InstrNodeWhile::setThen(InstrNode* n)
{
    scope = n;
}
InstrNodeWhile::~InstrNodeWhile()
{
    if (scope) delete scope;
    if (condition) delete condition;
}
InstrNode* InstrNodeWhile::accept(AbstractTraveller* tr)
{
    tr->visit(this);
    return next;
};



AbstractTraveller::~AbstractTraveller()
{

}

InstrNode* CodeParser::makeTree(std::vector<InstrToken> tokens)
{
    //std::cout<<"[New Tree]\n";
    auto len = tokens.size();
    InstrNode* start = new InstrStart;
    auto current = start;
    //std::cout<<len<<"\n";
    for (size_t i = 0; i < len;)
    {
        //if ( k < 15 ) std::cout<<k<<" "<<i<<" Type: "<<static_cast<int>(tokens[i].type)<<" Expr: "<<tokens[i].token<<"\n";
        if (tokens[i].type == InstrType::Expression)
        {
            auto newNode = makeNode(tokens[i++]);
            current->setNext(newNode);
            current = newNode;
        }
        else if (tokens[i].type == InstrType::Scope)
        {
            auto newNode = makeNode(tokens[i++]);
            current->setNext(newNode);
            current = newNode;
        }
        else if (tokens[i].type == InstrType::If)
        {
            auto newIf = new InstrNodeIf;
            i++;
            if ( tokens[i].type == InstrType::Expression )
            {
                newIf->setCondition( makeNode(tokens[i++]) );
            }
            if ( tokens[i].type == InstrType::Expression ||  tokens[i].type == InstrType::Scope )
            {
                newIf->setThen( makeNode(tokens[i++]) );
            }
            if (tokens[i].type == InstrType::Else)
            {
                ++i;
                if (len && (tokens[i].type == InstrType::Expression || tokens[i].type == InstrType::Scope))
                {
                    newIf->setElse(makeNode(tokens[i++]));
                }
            }
            current->setNext(newIf);
            current = newIf;
        }
        else if (tokens[i].type == InstrType::While)
        {
            auto newNode = new InstrNodeWhile;
            i++;
            newNode->setCondition(makeNode(tokens[i++]));
            newNode->setThen(makeNode(tokens[i++]));
            current->setNext(newNode);
            current = newNode;
        }
    }
    auto ret = start->getNext();
    start->setNext(nullptr);
    delete start;
    return ret;
}

InstrNode* makeNode(const InstrToken& node)
{
    if (node.type==InstrType::Expression)
    {
        auto N = new InstrNodeExpr(node.token);
        return N;
    }
    if (node.type==InstrType::Scope)
    {
        auto token = node.token;
        token.erase(token.length()-1,1);
        token.erase(0,1);
        CodeLexer l;
        CodeParser p;
        l.tokenize(token);
        auto code = l.get();
        auto scope = p.makeTree(code);

        auto N = new InstrNodeScope(scope);

        return N;
    }
    if (node.type==InstrType::If)
    {
        auto N = new InstrNodeIf;
        return N;
    }
    throw LangException{0,std::string("Unknown Token Type")};
}