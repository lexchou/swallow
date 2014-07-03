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
    
    Swift::Node* parse(const wchar_t* str)
    {
        using namespace Swift;
        Node::NodeCount = 0;
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        parser.setFileName(L"<file>");
        return parser.parse(str);
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
    
    void assertNodesEmpty(const char* file, int line)
    {
#ifdef TRACE_NODE
        using namespace Swift;
        using namespace std;
        if(Node::NodeCount != 0)
        {
            stringstream ss;
            set<Node*>::iterator iter = Node::UnreleasedNodes.begin();
            ss<<Node::NodeCount<<" unreleased AST nodes detected:";
            for(; iter != Node::UnreleasedNodes.end(); iter++)
            {
                Node* node = *iter;
                ss << static_cast<const void*>(node) <<", ";
            }
            CPPUNIT_NS::Asserter::fail(ss.str(), CPPUNIT_NS::SourceLine(file, line));
        }
#endif//TRACE_NODE
    }
};

#define DESTROY(n) delete n; assertNodesEmpty(__FILE__, __LINE__);

#endif//TEST_UTILS_H