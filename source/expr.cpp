#include "../headers/expr.h"
#include<stack>
#include<memory>
static const size_t NOT_FOUND = 0;

ExprElem::ExprElem(ExprType t) : type(t){}
ExprType ExprElem::getType(){ return type; }
void ExprElem::setParent(ExprElem* p){ parent = p; }
ExprElem* ExprElem::getParent(){ return parent; }
Object ExprElem::getValue(Frame * f){ return 0; }
ExprElem::~ExprElem()
{
    logger << " TYPE OF EXPR NODE " << (int)type;
    logger.log();
};
Object ExprElem::count(Frame * f)
{
    Object res = getValue(f);
    logger << res;
    logger.log();
    return res; 
}

ExprNumber::ExprNumber(std::string& s) : ExprElem(ExprType::Number)
{
    value = newObject(s);
    //std::cout<<value->toStdStr()<<"\n";
}
Object ExprNumber::getValue(Frame * f)
{
    //std::cout<<value->toStdStr()<<" Before Stop in GetValue"<<"\n";
    //std::cout<<" Before Stop in GetValue ret"<<"\n";
    return newObject(value);
}

ExprVariable::ExprVariable(std::string& s) : ExprElem(ExprType::Variable), value(s) {}


Object ExprVariable::getValue(Frame * f)
{
    return f->copyVar(value);
}
std::string ExprVariable::getName()
{
    return value;
}

void ExprOperation::setLeft( std::unique_ptr<ExprElem>&& el )
{
    left = std::move(el);
    left->setParent(this);
}
void ExprOperation::setRight( std::unique_ptr<ExprElem>&& el )
{
    right = std::move(el);
    right->setParent(this);
}

#if 0
ExprElem* ExprOperation::getRight( )
{
    return right;
}
ExprElem* ExprOperation::getLeft( )
{
    return left;
}
#endif
Object ExprOperation::getValue(Frame * f)
{
    if (op!="="){
        Object a = left->getValue(f);
        Object b = right->getValue(f);
        return ((*a).*operator_match.at(op))(b);
    }
    else
    {
        //std::cout<<"In assignment\n";
        //system("pause");
        if ( left->getType() != ExprType::Variable ) throw LangException{0,"Left Operand isn't a variable"};
        auto var = static_cast<ExprVariable*>(left.get());
        
        //std::cout<<"Var Name: \n"<<var->getName()<<"\n";
        //system("pause");

        Object b = right->getValue(f);
        //std::cout<<"Value: "<<b->toStdStr()<<" "<<b<<"\n";
        //system("pause");
        f->setVar(var->getName(),b);
        //std::cout<<"Value moved: ";//<<b->toStdStr()<<" "<<b<<"\n";
        //system("pause");
        return f->copyVar(var->getName());
    }
}
ExprOperation::ExprOperation(const std::string& op) : ExprElem(ExprType::Operation), op(op){}
ExprOperation::~ExprOperation()
{
}

void ExprUnarOperation::setChild( std::unique_ptr<ExprElem>&& el )
{
    child = std::move(el);
    child->setParent(this);
}

Object ExprUnarOperation::getValue(Frame * f)
{
    if (unary_operator_match.contains(op)){
        auto fp = unary_operator_match.at(op);
        auto a = child->getValue(f);
        return ((*a).*fp)();
    }
    throw LangException(0,std::string( "Unknown Unary Operation" ) + op);
}

ExprUnarOperation::ExprUnarOperation(const std::string& op) : ExprElem(ExprType::UnarOperation), op(op){}

ExprUnarOperation::~ExprUnarOperation()
{
}

////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////

Token::Token(const std::string& v,TokenType t): value(v),type(t){}

void Token::print() const
{
    std::cout<<"Тип: "<<(int)type<<", Значение: \'"<<value<<"\' "<<'\n';
}

void Lexer::update()
{
    result.emplace_back(currentToken,currentType);
    currentToken.clear();
    currentType = TokenType::Null;
}
std::vector<Token> Lexer::tokenize(const std::string& s)
{
    size_t parenCounter = 0;
    std::unordered_set<TokenType> expections;
    result.reserve(5);
    auto len = s.length();
    size_t i = 0;
    while (i<len)
    {
        if (s[i]==' ') {i++; continue;}

        if ( isDigit(s[i]) && parenCounter==0)
        {
            while (  i<len  && (isDigit(s[i]) || s[i]=='.'))
            {
                currentToken+=s[i++];
            }
            currentType = TokenType::Number;
            update();
            continue;
        }
        else if (s[i] == '\"' && parenCounter==0)
        {
            currentToken = "\"";
            i++;
            while (i < len && s[i] != '\"') {
                currentToken += s[i++];
            }
            if (i >= len) throw LangException{0, "Unterminated string"};
            currentToken += '\"';
            i++;
            currentType = TokenType::StringValue;
            //system("pause");
            update();
            continue;
        }
        else if (isOperator(s[i]) && parenCounter == 0)
        {
            currentToken += s[i++];

            while (i < len && isOperator(s[i]) &&
                (binary_operators.contains(currentToken + s[i]) ||
                    unary_operators.contains(currentToken + s[i])))
            {
                currentToken += s[i++];
            }

            currentType = TokenType::UnaryOperator;
            if (!result.empty() &&
                result.back().type != TokenType::BinaryOperator &&
                result.back().type != TokenType::UnaryOperator)
            {
                currentType = TokenType::BinaryOperator;
            }
            update();
            continue;
        }
        else if ( isLetter(s[i]) && parenCounter==0 )
        {
            while (i<len && ( isLetter(s[i]) || s[i]=='.' || isDigit(s[i])) )
            {
                currentToken+=s[i++];
            }
            if (string_value.contains(currentToken))
            {
                currentType = TokenType::StringValue;
                update();
                continue;
            }
            if ( i<len && open_parantheses_match.contains(std::string(1,s[i])))
            {
                currentType=TokenType::Function;
                continue;
            }
            else
            {
                currentType = TokenType::Variable;
                update();
                continue;
            }
        }
        else if ( open_parantheses_match.contains(std::string(1,s[i])) || parenCounter>0 )
        {
            //std::cout<<"Start of IF "<<i<<" Letter: "<< s[i]<<" PC: "<<parenCounter<<"\n";
            parenCounter++;
            currentToken+=s[i++];
            //std::cout<<"After Increment "<<i<<" Letter: "<< s[i]<<" PC: "<<parenCounter<<"\n";
            if ( currentType==TokenType::Null ) currentType=TokenType::Expression;
            while ( i<len && parenCounter>0 )
            {
                if ( s[i]==' ') i++;
                if (open_parantheses_match.contains(std::string(1,s[i]))) parenCounter++;
                if (close_parantheses_match.contains(std::string(1,s[i]))) parenCounter--;
                currentToken+=s[i++];
            }
            update();
            continue;
        }
        else
        {
            throw LangException{0,std::string("Unknown LexerError ") + s[i]};
        }
    }
    return result;
}

////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////

Expression::Expression(const std::string & expr)
{
    nexpr = l.tokenize(expr);
}

Expression::Expression(const std::vector<Token>& tokens,size_t i,size_t j)
{
    nexpr.reserve(j-i);
    while (i<j)
    {
        nexpr.emplace_back(tokens[i++]);
    }
}

size_t Expression::find_next_operator()
{
    size_t pos = 0;
    short min_weight = 127;
    for (size_t i = nexpr.size()-1 ; i>0 ;i--)
    {
        if ( nexpr[i].type == TokenType::BinaryOperator )
        {

            if ( operators_weigth.contains(nexpr[i].value) && ( ( min_weight != 0 && operators_weigth.at(nexpr[i].value) < min_weight) || ( min_weight == 0 && operators_weigth.at(nexpr[i].value) == 0 )))
            {
                min_weight = operators_weigth.at(nexpr[i].value);
                pos = i;
            }
        }
    }
    return pos;
}

void Expression::unParenth()
{
    if (!(nexpr.size()==1 && nexpr[0].value[0] == '(' )) return;
    nexpr[0].value.erase(nexpr[0].value.length()-1,1);
    nexpr[0].value.erase(0,1);
    //nexpr = l.tokenize( nexpr[0].value );
}

ExprElem* Expression::makeTree()
{
    unParenth();
    auto next = find_next_operator();

    // for (auto& t: nexpr)
    // {
    //     t.print();
    // }
    // if (nexpr.empty()) std::cout<<"NEXPR IS EMPTY\n";
    // system("pause");

    auto type = nexpr[next].type;

    if (next!=NOT_FOUND)
    {
        auto operNode = new ExprOperation(nexpr[next].value);
        Expression L = Expression(nexpr,0,next);
        Expression R = Expression(nexpr,next+1,nexpr.size());
        operNode->setLeft( std::unique_ptr<ExprElem>(L.makeTree()) );
        operNode->setRight( std::unique_ptr<ExprElem>(R.makeTree()) );
        return operNode;
    }
    else if (type==TokenType::Expression)
    {
        Expression X = Expression(nexpr[0].value);
        return X.makeTree();
    }
    else if (type==TokenType::UnaryOperator)
    {
        auto unaryNode = new ExprUnarOperation( nexpr[next].value );
        Expression C = Expression( nexpr, 1,nexpr.size() );
        unaryNode->setChild( std::unique_ptr<ExprElem>(C.makeTree()) );
        return unaryNode;
    }
    else if (type==TokenType::Number || type==TokenType::StringValue)
    {
        auto numNode = new ExprNumber(nexpr[next].value);
        return numNode;
    }
    else if (type==TokenType::Variable)
    {
        logger.log();
        return new ExprVariable(nexpr[next].value);
    }
    else
    {
        nexpr[next].print();
        throw LangException{0,std::string("WrongType")};
    }
}