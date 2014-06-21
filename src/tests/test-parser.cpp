#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cstdlib>
using namespace Swift;

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


class TestExpression : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestExpression);
    CPPUNIT_TEST(testLiteral);
    CPPUNIT_TEST_SUITE_END();
public:
   
    void testLiteral()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        Node* root = parser.parse(L"3");
        CPPUNIT_ASSERT(root != NULL);
        IntegerLiteral* n = dynamic_cast<IntegerLiteral*>(root);
        CPPUNIT_ASSERT(n == NULL);

        //ASSERT_EQUALS(L"3", n->toString());

    }
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestExpression, "alltest");
int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &alltest = CppUnit::TestFactoryRegistry::getRegistry("alltest");
    runner.addTest(alltest.makeTest());
    bool failed = !runner.run();
    return failed;
}
