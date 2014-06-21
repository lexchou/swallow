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

class TestOperatorExpression : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestOperatorExpression);
    CPPUNIT_TEST(testAdd);
    CPPUNIT_TEST_SUITE_END();
public:
    void testAdd()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        Node* root = parser.parse(L"3+4*5");
        CPPUNIT_ASSERT(root != NULL);
        BinaryOperator* n = dynamic_cast<BinaryOperator*>(root);
        CPPUNIT_ASSERT(n != NULL);
        CPPUNIT_ASSERT_EQUAL(2, n->numChildren());
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestOperatorExpression, "alltest");


class TestLiteralExpression : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLiteralExpression);
    CPPUNIT_TEST(testLiteral);
    CPPUNIT_TEST(testCompileConstants);
    CPPUNIT_TEST(testArrayLiteral);
    CPPUNIT_TEST(testDictionaryLiteral);
    CPPUNIT_TEST_SUITE_END();
public:
   
    void testLiteral()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        parser.setFileName(L"<file>");
        
        Node* root = parser.parse(L"3");
        CPPUNIT_ASSERT(root != NULL);
        IntegerLiteral* n = dynamic_cast<IntegerLiteral*>(root);
        CPPUNIT_ASSERT(n != NULL);
        ASSERT_EQUALS(L"3", n->toString().c_str());
        
        root = parser.parse(L"3.45e4");
        CPPUNIT_ASSERT(root != NULL);
        FloatLiteral* f = dynamic_cast<FloatLiteral*>(root);
        CPPUNIT_ASSERT(f != NULL);
        ASSERT_EQUALS(L"3.45e4", f->toString().c_str());
        
        
        root = parser.parse(L"\"str\\thello\"");
        CPPUNIT_ASSERT(root != NULL);
        StringLiteral* str = dynamic_cast<StringLiteral*>(root);
        CPPUNIT_ASSERT(str != NULL);
        ASSERT_EQUALS(L"str\thello", str->toString().c_str());
        
    }
    void testArrayLiteral()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        
        Node* root = parser.parse(L"[]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral* a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(0, a->numChildren());
        
        root = parser.parse(L"[5]");
        CPPUNIT_ASSERT(root != NULL);
        a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(1, a->numChildren());
        IntegerLiteral* i = dynamic_cast<IntegerLiteral*>(a->get(0));
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"5", i->toString().c_str());
        
        
        root = parser.parse(L"[\"a\",]");
        CPPUNIT_ASSERT(root != NULL);
        a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(1, a->numChildren());
        StringLiteral* s = dynamic_cast<StringLiteral*>(a->get(0));
        CPPUNIT_ASSERT(s != NULL);
        ASSERT_EQUALS(L"a", s->toString().c_str());
        
        root = parser.parse(L"[5,6]");
        CPPUNIT_ASSERT(root != NULL);
        a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a);
        CPPUNIT_ASSERT_EQUAL(2, a->numChildren());
        
        root = parser.parse(L"[5,6, [5]]");
        CPPUNIT_ASSERT(root != NULL);
        a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a);
        CPPUNIT_ASSERT_EQUAL(3, a->numChildren());
        
    }
    void testDictionaryLiteral()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        
        Node* root = parser.parse(L"[:]");
        CPPUNIT_ASSERT(root != NULL);
        DictionaryLiteral* d = dynamic_cast<DictionaryLiteral*>(root);
        CPPUNIT_ASSERT(d != NULL);
        CPPUNIT_ASSERT_EQUAL(0, d->numChildren());
        
        root = parser.parse(L"[5 : 1]");
        CPPUNIT_ASSERT(root != NULL);
        d = dynamic_cast<DictionaryLiteral*>(root);
        CPPUNIT_ASSERT(d != NULL);
        CPPUNIT_ASSERT_EQUAL(2, d->numChildren());

        
    }
    void testCompileConstants()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        parser.setFileName(L"<file>");
    
        Node* root = parser.parse(L"__FILE__");
        CPPUNIT_ASSERT(root != NULL);
        CompileConstant* c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"<file>", c->getValue().c_str());
        
        
        root = parser.parse(L"__LINE__");
        CPPUNIT_ASSERT(root != NULL);
        c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"1", c->getValue().c_str());
        
        
        root = parser.parse(L"__COLUMN__");
        CPPUNIT_ASSERT(root != NULL);
        c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"1", c->getValue().c_str());
        
        
        root = parser.parse(L"__FUNCTION__");
        CPPUNIT_ASSERT(root != NULL);
        c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"<top>", c->getValue().c_str());
    }
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestLiteralExpression, "alltest");
int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &alltest = CppUnit::TestFactoryRegistry::getRegistry("alltest");
    runner.addTest(alltest.makeTest());
    bool failed = !runner.run();
    return failed;
}
