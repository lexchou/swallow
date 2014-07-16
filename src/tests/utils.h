#ifndef TEST_UTILS_H
#define TEST_UTILS_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cstdlib>
#include "parser/parser.h"
#include "semantics/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"
#include "common/compiler_results.h"


#define ASSERT_EQUALS(E, A) wcs_assertEquals((E), (A), __FILE__, __LINE__)

class SwiftTestCase : public CppUnit::TestFixture
{
public:
    Swift::Node* analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
    {
        Swift::Node* ret = parseStatement(registry, compilerResults, func, str);
        return ret;
    }
    Swift::Node* parseStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
    {
        using namespace Swift;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &registry, &compilerResults);
        parser.setFileName(L"<file>");
        Node* ret = parser.parse(str);
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
                }
                std::wcout<<L"("<<res.line<<L", "<<res.column<<") ";
                std::wstring msg = compilerResults.format(res);
                std::wcout<<msg<<std::endl;
            }

        }

        return ret;
    }
    void wcs_assertEquals(const wchar_t* expected, const std::wstring& actual, const char* file, int line)
    {
        wcs_assertEquals(expected, actual.c_str(), file, line);
    }
    void wcs_assertEquals(const wchar_t* expected, const wchar_t* actual, const char* file, int line)
    {
        if(!wcscmp(expected, actual))
            return;
        char expected2[1024];
        char actual2[1024];
        wcstombs(expected2, expected, sizeof(expected2));
        wcstombs(actual2, actual, sizeof(actual2));
        
        
        CPPUNIT_NS::Asserter::failNotEqual( expected2, actual2, CPPUNIT_NS::SourceLine(file, line), "");
    }
};

struct Tracer
{
    Tracer(const char* file, int line, const char* func)
    {
        strcpy(this->file, file);
        strcpy(this->func, func);
        this->line = line;
        node = NULL;
#ifdef TRACE_NODE
        using namespace Swift;
        Node::UnreleasedNodes.clear();
        Node::NodeCount = 0;
#endif
    }
    ~Tracer()
    {
        delete node;
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
                ss << static_cast<const void*>(node) <<", ";
            }
            std::cout<<ss.str()<<std::endl;
        }
#endif//TRACE_NODE
    }
    
    Swift::Node* node;
    char file[1024];
    int line;
    char func[100];
    
};
#define PARSE_STATEMENT(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::SymbolRegistry symbolRegistry; \
    Swift::CompilerResults compilerResults; \
    Node* root = parseStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    tracer.node = root;
#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::SymbolRegistry symbolRegistry; \
    Swift::CompilerResults compilerResults; \
    Node* root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    tracer.node = root; \
    Swift::SymbolScope* scope = symbolRegistry.getCurrentScope();


#endif//TEST_UTILS_H



