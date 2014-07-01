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
        return parser.parse(str);
    }
};

#define DESTROY(n) delete n; CPPUNIT_ASSERT_EQUAL(0, Node::NodeCount);

#endif//TEST_UTILS_H