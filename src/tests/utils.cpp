#include "utils.h"

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
            ss << static_cast<const void*>(node) << "(" << nodeName << ")" <<", ";
        }
        std::cout<<ss.str()<<std::endl;
    }
#endif//TRACE_NODE
}
