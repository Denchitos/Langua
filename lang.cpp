#include"headers/interpreter.h"
#include"headers/executors.h"
#include "fstream"

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
        catch(LangException& le)
        {
            std::cout<<le.getCause()<<"\n";
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



int main(int argc,char* argv[])
{
    #ifdef _WIN32
        system("chcp 65001");
    #endif
    if ( argc< 2)
    {
        std::cerr<<"Error! Expected:\nlang.exe <file> <flags...>";
        return 1;
    }
    std::string filename = argv[1];
    if (filename=="test")
    {
        test();
        return 0;
    }
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
    else
    {
        std::cerr << "Unknown Error! Can't open file. : "<<filename<< "\n";
        return 2;
    }
    //std::cout<<filename<<": [[[\n"<<text<<"\n]]]\n\n\n";
    code.close();
    CodeLexer l;
    erase(text,'\n');
    //erase(text,' ');
    try{
        l.tokenize(text);
        auto instructions = l.get();
        CodeParser p;
        std::unique_ptr<InstrNode> x (p.makeTree(instructions));
        PrintTraveller t;
        ExecutorTraveller e;
        std::cout<<"AST TREE:\n{\n";
        t.start(x.get());
        std::cout<<"}\n";
        std::cout<<"\n\n\n[Start]\n";
        e.start(x.get());
    }
    catch(LangException& e)
    {
        std::cerr<<e.getCause()<<"\n";
    }
    catch(std::exception& e)
    {
        std::cerr<<e.what()<<"\n";
    }
    std::cout<<"\n[Stop]\n";
    system("pause");
}
