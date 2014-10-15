#include "utils.h"
#include "ast/NodeSerializer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodeFactory.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include <sstream>
#include <fstream>
//#include <codecvt>


std::wstring readFile(const char* fileName)
{
    std::wifstream wif(fileName);
    //wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

void dumpCompilerResults(Swift::CompilerResults& compilerResults)
{
    using namespace Swift;
    if(compilerResults.numResults() > 0)
    {
        for(int i = 0; i < compilerResults.numResults(); i++)
        {
            const CompilerResult& res = compilerResults.getResult(i);
            switch(res.level)
            {
                case ErrorLevel::Fatal:
                    std::wcout<<L"Fatal:";
                    break;
                case ErrorLevel::Error:
                    std::wcout<<L"Error:";
                    break;
                case ErrorLevel::Warning:
                    std::wcout<<L"Warning:";
                    break;
                case ErrorLevel::Note:
                    std::wcout<<L"Note:";
                    break;
            }
            std::wcout<<L"("<<res.line<<L", "<<res.column<<") ";
            std::wstring msg = compilerResults.format(res);
            std::wcout<<msg<<std::endl;
        }

    }
}


Swift::NodePtr parseStatement(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    NodePtr ret = parser.parseStatement(str);
    dumpCompilerResults(compilerResults);
    return ret;
}

Swift::ProgramPtr parseStatements(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ProgramPtr ret = parser.parse(str);
    dumpCompilerResults(compilerResults);
    return ret;
}
Swift::ScopedProgramPtr analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;

    FunctionOverloadedSymbolPtr println(new FunctionOverloadedSymbol());
    {
        TypePtr t_int = registry.lookupType(L"Int");
        std::vector<Type::Parameter> parameters = {Type::Parameter(t_int)};
        TypePtr type = Type::newFunction(parameters, nullptr, false);
        FunctionSymbolPtr println_int(new FunctionSymbol(L"println", type, nullptr));
        println->add(println_int);
    }
    registry.getCurrentScope()->addSymbol(L"println", SymbolPtr(println));
    ScopedNodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(str));
//ScopedProgram* f;
    //cdumpCompilerResults(compilerResults);
//d.g();
    if(!ret)
        return ret;
    try
    {
        SemanticAnalyzer analyzer(&registry, &compilerResults);
        ret->accept(&analyzer);
    }
    catch(const Abort&)
    {
//ignore this
    }

    return ret;
}


Tracer::Tracer(const char* file, int line, const char* func)
{
    strcpy(this->file, file);
    strcpy(this->func, func);
    this->line = line;
#ifdef TRACE_NODE
    using namespace Swift;
    Node::UnreleasedNodes.clear();
    Node::NodeCount = 0;
#endif
}
Tracer::~Tracer()
{
#ifdef TRACE_NODE
    using namespace Swift;
    using namespace std;
    list<Node*>& nodes = Node::UnreleasedNodes;
    int unreleasedNodes = nodes.size();
    if(Node::NodeCount != 0)
    {
        stringstream ss;
        list<Node*>::iterator iter = nodes.begin();
        ss<<unreleasedNodes<<" unreleased AST nodes detected in [" << file << ":" << func << "]:";
        for(; iter != nodes.end(); iter++)
        {
            Node* node = *iter;
            const char* nodeName = Node::nodeTypeToName(node->getNodeType());
            NodeSerializerA serializer(ss);
            ss << nodeName << " : " << static_cast<const void*>(node) << "(";
            node->accept(&serializer);
            ss << ")" << ", ";
        }
        std::cout<<ss.str()<<std::endl;
    }
#endif//TRACE_NODE
}
