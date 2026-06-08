#include "headers/init.h"
#include "headers/expr.h"
void test()
{
    std::string my_expr;
    //setDebugMode();
    while (true)
    {
        std::cout << ">>>";
        std::getline( std::cin,my_expr );
        if (my_expr == "QUIT")
        {
            break;
        }
        else if (my_expr == "PRINT")
        {
            consoleLogVars();
            continue;
        }
        //std::cout<<my_expr<<"\n";
        try
        {
            Lexer l = Lexer{};
            auto t = l.tokenize(my_expr);
            for (auto& i: t)
            {
                i.print();
            }
            Expression r(my_expr);
            std::unique_ptr<ExprElem> x(r.makeTree());
            if (x) x->count();
        }
        catch(LangException le)
        {
            std::cout<<le.cause<<"\n";
        }
        catch(std::exception& err)
        {
            std::cout<<err.what()<<"\n";
        }
        catch( ... )
        {
            std::cout<<"Unknown exception\n";
        }
    }
}

int main(int argc, char* argv[])
{
    //system("chcp 1251");
    std::cout<<argc<<"\n";
    for (int i=1; i<argc;i++)
    {
        if (std::string(argv[i]) == "-debug") setDebugMode(true);
        std::cout<<argv[i]<<"\n";
    }
    test();
    return 0;
}
