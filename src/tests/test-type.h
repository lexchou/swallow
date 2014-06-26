#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

using namespace Swift;

class TestType : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestType);
    CPPUNIT_TEST_SUITE_END();
public:
    
    Node* parse(const wchar_t* str)
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        return parser.parse(str);
    }
    
    void testFor()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestType, "alltest");



#endif