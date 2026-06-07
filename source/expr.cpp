#include "../headers/expr.h"
#include<stack>

ExprElem::ExprElem(ExprType t) : type(t){}
ExprType ExprElem::getType(){ return type; }
void ExprElem::setParent(ExprElem* p){ parent = p; }
ExprElem* ExprElem::getParent(){ return parent; }
Object ExprElem::getValue(){ return 0; }
ExprElem::~ExprElem()
{
    logger << " TYPE OF EXPR NODE " << (int)type;
    logger.log();
};
Object ExprElem::count()
{
    Object res = getValue();
    logger << res;
    logger.log();
    return res; 
}

ExprNumber::ExprNumber(std::string& s) : ExprElem(ExprType::Number)
{
    if ( s.find('.') == std::string::npos ) value = newInt( stoi(s) );
    if ( s.find('.') != std::string::npos ) value = newDouble( stod(s) );
}
Object ExprNumber::getValue()
{
    return std::move(value);
}

ExprVariable::ExprVariable(std::string& s) : ExprElem(ExprType::Variable), value(s) {}


Object ExprVariable::getValue()
{
    return copyVar(value);
}
std::string ExprVariable::getName()
{
    return value;
}

void ExprOperation::setLeft( ExprElem* el )
{
    left = el;
    left->setParent(this);
}
void ExprOperation::setRight( ExprElem* el )
{
    right = el;
    right->setParent(this);
}
ExprElem* ExprOperation::getRight( )
{
    return right;
}
ExprElem* ExprOperation::getLeft( )
{
    return left;
}
Object ExprOperation::getValue()
{
    if (op!="="){
        Object a = left->getValue();
        Object b = right->getValue();
        return ((*a).*operator_match.at(op))(b);
    }
    else
    {
        if ( left->getType() != ExprType::Variable ) throw LangException{0,"Left Operand isn't a variable"};
        auto var = static_cast<ExprVariable*>(left);
        Object b = right->getValue();
        logger<<b;
        logger.log();
        setVar(var->getName(),b);
        return copyVar(var->getName());
    }
}
ExprOperation::ExprOperation(const std::string& op) : ExprElem(ExprType::Operation), op(op){}
ExprOperation::~ExprOperation()
{
    if (right) delete right;
    if (left) delete left;
}

void ExprUnarOperation::setChild( ExprElem* el )
{
    child = el;
    child->setParent(this);
}

Object ExprUnarOperation::getValue()
{
    if (unary_operator_match.contains(op)){
        auto fp = unary_operator_match.at(op);
        auto a = child->getValue();
        return ((*a).*fp)();
    }
    throw LangException(0,std::string( "Unknown Unary Operation" ) + op);
}

ExprElem* ExprUnarOperation::getChild( )
{
    return child;
}

ExprUnarOperation::ExprUnarOperation(const std::string& op) : ExprElem(ExprType::UnarOperation), op(op){}

ExprUnarOperation::~ExprUnarOperation()
{
    if (child) delete child;
}

std::vector<std::string> tokenize_expr(const std::string& _expr)
{
    std::stack<std::string> stackParanth;
    std::vector<std::string> res = { "" };
    size_t k = 0;
    res.reserve(5);

    bool in_operator = false;
    for ( int i = 0;i<_expr.length();i++)
    {
        char c = _expr[i];
        if (c!=' ')
        {          
            if ( open_parantheses_match.contains(std::string(1,c)) )
            {
                stackParanth.push( open_parantheses_match.at(std::string(1,c)) );
            }
            else if ( close_parantheses_match.contains(std::string(1,c)) && stackParanth.top() != std::string(1,c) )
            {
                throw LangException{0,std::string( "There paranthese was lost :" ) + c};
            }
            else if ( close_parantheses_match.contains(std::string(1,c)) && stackParanth.top() == std::string(1,c) )
            {
                stackParanth.pop();
            }
            if ( stackParanth.empty() && !in_operator && (operator_symbols.find(c) != std::string::npos) )
            {
                logger << c <<" <-To Next Token\n";
                in_operator = true;
                k++;
                res.push_back("");
            }
            else if ( in_operator && (operator_symbols.find(c) == std::string::npos) )
            {
                logger << c <<" <-To Next Token\n";
                res.push_back("");
                k++;
                in_operator = false;
            }
            else if ( in_operator && (operator_symbols.find(c) != std::string::npos) )
            {
                if ( !operators_weigth.contains(res[k]+c))
                {
                    logger << c <<" <-To Next Token\n";
                    res.push_back("");
                    k++;
                }
            }
            res[k]+=c;
            logger.log();
        }
    }
    if (!stackParanth.empty()) throw LangException( 0,"Open and Close paratheses aren't matching" );
    if (res[0]=="") res.erase(res.begin());
    for (size_t i = 0;i<res.size();i++)
    {
        logger<<"[" <<i<<"] \'"<<res[i]<<"\'\n";
    }
    logger << "\n";
    logger.log();
    return res;
}

////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////NEWTOKENIZER////

Token::Token(const std::string& v,TokenType t): value(v),type(t){}

void Token::print() const
{
    std::cout<<"Type: "<<(int)type<<", Value: \'"<<value<<"\' "<<'\n';
}

bool isLetter(char c)
{
    return  letters.find(c) != std::string::npos; 
}

bool isDigit(char c)
{
    return  numbers.find(c) != std::string::npos; 
}

bool isOperator(char c)
{
    return  operator_symbols.find(c) != std::string::npos;
}


void Lexer::update()
{
    result.emplace_back(currentToken,currentType);
    currentToken.clear();
    currentType = TokenType::Null;
}
std::vector<Token> Lexer::new_tokenize_expr(const std::string& s)
{
    size_t parenCounter = 0;
    std::unordered_set<TokenType> expections;
    result.reserve(5);
    auto len = s.length();
    auto last = len-1;
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
        else if( isOperator(s[i]) && parenCounter==0 )
        {
            while (i<len && isOperator(s[i]) && ( binary_operators.contains(currentToken+s[i]) || unary_operators.contains(currentToken+s[i]) ) )
            {
                currentToken+=s[i++];
            }
            currentType = TokenType::UnaryOperator;
            if ( !result.empty() && result.back().type!=TokenType::BinaryOperator ) currentType=TokenType::BinaryOperator;
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

Expression::Expression(const std::vector<std::string>& tokens,int i,int j)
{
    for (int s = i;s<j;s++)
    {
        nexpr.push_back(tokens[s]);
    }
};

Expression::Expression(const std::string& _expr)
{
    nexpr = tokenize_expr(_expr);
};

bool Expression::isVar(bool mustFindOperator)
{
    if (nexpr.size()!=1) return false;
    if (mustFindOperator)
    {
        auto op = find_next_operator();
        if (op) return false;
    }
    return canBeVar(nexpr[0]);
}

size_t Expression::find_next_operator()
{
    unParanth();
    size_t next_op_pos = 0;
    size_t in_parantheses = 0;
    short min_weight = 255;
    for (size_t i = 1; i<nexpr.size() ; i++)
    {
        std::string c = nexpr[i];
        logger<<"\'"<<c<<"\' - ";
        if (operators_weigth.contains(c) && min_weight > operators_weigth.at(c))
        {
            min_weight = operators_weigth.at(c);
            next_op_pos = i;
        }
    }
    logger.log();
    return next_op_pos;
}

void Expression::unParanth()
{
    if (nexpr.size() == 1 && nexpr[0][0] == '(')
    {
        logger << "MUST UNPARANTH\n";
        size_t paranth_counter = 1;
        for (size_t i = 1;i<nexpr[0].length()-1;i++)
        {
            switch (nexpr[0][i])
            {
                case '(': paranth_counter++;break;
                case ')': paranth_counter--;break;
            }
            if ( paranth_counter==0) return;
        }
        nexpr[0].erase(nexpr[0].length()-1,1);
        nexpr[0].erase(0,1);
        logger<<"AFTER UNPARANTH{\n";
        logger<<nexpr[0]<<"\n}\n";
        logger.log();
        nexpr = std::move(tokenize_expr(nexpr[0]));
    }
}
// size_t Expression::log()
// {
//     size_t op = find_next_operator();
//     return op;
// }

void Expression::print()
{
    logger<<"[EXPRESSION]{\n";
    for (auto& i : nexpr)
    {
        logger << i<<' ';
    }
    logger<<"\n}";
    logger.log();
}

ExprElem* Expression::makeTree()
{
    unParanth();
    print();
    size_t div = find_next_operator();
    logger<<"<- "<<div<<"\n";
    logger.log();
    if (div!=0)
    {
        logger << "[BINARY OPERATION DETECTED] "<<nexpr[div]<<"\n";
        logger << nexpr[0] << " "<<nexpr[div] << " "<<nexpr[div+1] << nexpr[nexpr.size()-1] << "\n";
        auto operNode = new ExprOperation(nexpr[div]);
        Expression L = Expression(nexpr,0,div);
        Expression R = Expression(nexpr,div+1,nexpr.size());
        operNode->setLeft( L.makeTree() );
        operNode->setRight( R.makeTree() );
        logger.log();
        return operNode;
    }
    else if ( nexpr.size()!=1 )
    {
        logger << "[UNARY OPERATION DETECTED] "<<nexpr[0]<<"\n";
        auto UnNode = new ExprUnarOperation(nexpr[0]);
        Expression C = Expression( nexpr, 1,nexpr.size() );
        UnNode->setChild( C.makeTree() );
        logger.log();
        return UnNode;
    }
    else if (!isVar())
    {
        logger << "[SIMPLE NUMBER DETECTED] "<<nexpr[0]<<"\n";;
        auto numNode = new ExprNumber(nexpr[0]);
        logger.log();
        return numNode;
    }
    else if (isVar())
    {
        logger << "[VARIABLE DETECTED] "<<nexpr[0]<<"\n";;
        logger.log();
        return new ExprVariable(nexpr[0]);
    }
    else throw LangException{0,"WrongException"};
}