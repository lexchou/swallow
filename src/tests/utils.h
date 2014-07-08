#ifndef TEST_UTILS_H
#define TEST_UTILS_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cstdlib>
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"


#define ASSERT_EQUALS(E, A) wcs_assertEquals((E), (A), __FILE__, __LINE__)

class SwiftTestCase : public CppUnit::TestFixture
{
public:
    
    Swift::Node* parseStatement(const wchar_t* str)
    {
        using namespace Swift;
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        parser.setFileName(L"<file>");
        return parser.parse(str);
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
#ifdef TRACE_NODE
        using namespace Swift;
        Node::UnreleasedNodes.clear();
        Node::NodeCount = 0;
#endif
    }
    ~Tracer()
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
                ss << static_cast<const void*>(node) <<", ";
            }
            std::cout<<ss.str()<<std::endl;
        }
#endif//TRACE_NODE
    }
    
    
    char file[1024];
    int line;
    char func[100];
    
};
#define PARSE_STATEMENT(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__);Node* root = parseStatement((s));



#define DESTROY(n) delete n;

#endif//TEST_UTILS_H