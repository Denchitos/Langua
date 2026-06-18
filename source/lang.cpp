#include"../headers/interpreter.h"
#include"../headers/executors.h"
#include "fstream"

//#define MENU

#ifdef MENU
void test()
{
    std::string my_expr;
    //setDebugMode();
    Frame f;
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
            f.consoleLogVars();
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
            if (x) x->count(&f);
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
    system("chcp 65001");
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
#endif

#ifndef MENU
int main()
{
    std::string filename = "code.txt";
    std::ifstream code(filename);
    std::string text;
    if (code.is_open())
    {
        std::string line;
        while(getline(code,line))
        {
            text+=line;
            text+='\n';
        }
    }
    std::cout<<filename<<": [[[\n"<<text<<"\n]]]\n\n\n";
    code.close();
    CodeLexer l;
    erase(text,'\n');
    //erase(text,' ');
    l.tokenize(text);
    auto instructions = l.get();
    std::cout<<"Tokens[\n";
    for (auto& x:instructions)
    {
        std::cout<<static_cast<int>(x.type)<<':'<<x.token<<"\n";
    }
    std::cout<<"]\n";

    CodeParser p;
    std::unique_ptr<InstrNode> x ( p.makeTree(instructions) );
    PrintTraveller t;
    ExecutorTraveller e;
    std::cout<<"Start Adventure!\n";
    t.start(x.get());
    std::cout<<"End Adventure :(\n";
    std::cout<<"\n\n\nStart:\n";
    try{
        e.start(x.get());
    }
    catch(LangException& e)
    {
        std::cout<<e.cause<<"\n";
    }
    catch(std::exception& e)
    {
        std::cout<<e.what()<<"\n";
    }
    std::cout<<"Stop\n";
    system("pause");
}
#endif
