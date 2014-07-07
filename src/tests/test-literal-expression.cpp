#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

using namespace Swift;

class TestLiteralExpression : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestLiteralExpression);
    CPPUNIT_TEST(testLiteral);
    CPPUNIT_TEST(testLiteral2);
    CPPUNIT_TEST(testLiteral3);
    CPPUNIT_TEST(testCompileConstants);
    CPPUNIT_TEST(testCompileConstants2);
    CPPUNIT_TEST(testCompileConstants3);
    CPPUNIT_TEST(testCompileConstants4);
    CPPUNIT_TEST(testArrayLiteral);
    CPPUNIT_TEST(testArrayLiteral2);
    CPPUNIT_TEST(testArrayLiteral3);
    CPPUNIT_TEST(testArrayLiteral4);
    CPPUNIT_TEST(testArrayLiteral5);
    CPPUNIT_TEST(testDictionaryLiteral);
    CPPUNIT_TEST(testDictionaryLiteral2);
    CPPUNIT_TEST_SUITE_END();
public:
    void testLiteral()
    {
        
        PARSE_STATEMENT(L"3");
        CPPUNIT_ASSERT(root != NULL);
        IntegerLiteral* n = dynamic_cast<IntegerLiteral*>(root);
        CPPUNIT_ASSERT(n != NULL);
        ASSERT_EQUALS(L"3", n->toString().c_str());
        
        DESTROY(root);
    }
    void testLiteral2()
    {
        PARSE_STATEMENT(L"3.45e4");
        CPPUNIT_ASSERT(root != NULL);
        FloatLiteral* f = dynamic_cast<FloatLiteral*>(root);
        CPPUNIT_ASSERT(f != NULL);
        ASSERT_EQUALS(L"3.45e4", f->toString().c_str());
        
        DESTROY(root);
    }
    
    void testLiteral3()
    {
        PARSE_STATEMENT(L"\"str\\thello\"");
        CPPUNIT_ASSERT(root != NULL);
        StringLiteral* str = dynamic_cast<StringLiteral*>(root);
        CPPUNIT_ASSERT(str != NULL);
        ASSERT_EQUALS(L"str\thello", str->toString().c_str());
        
        DESTROY(root);
        
    }
    void testArrayLiteral()
    {
        PARSE_STATEMENT(L"[]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral* a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(0, a->numElements());
        
        
        DESTROY(root);
    }
    
    void testArrayLiteral2()
    {
        PARSE_STATEMENT(L"[5]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral* a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(1, a->numElements());
        IntegerLiteral* i = dynamic_cast<IntegerLiteral*>(a->getElement(0));
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"5", i->toString().c_str());
        
        
        DESTROY(root);
    }
    void testArrayLiteral3()
    {
        PARSE_STATEMENT(L"[\"a\",]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral* a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a != NULL);
        CPPUNIT_ASSERT_EQUAL(1, a->numElements());
        StringLiteral* s = dynamic_cast<StringLiteral*>(a->getElement(0));
        CPPUNIT_ASSERT(s != NULL);
        ASSERT_EQUALS(L"a", s->toString().c_str());
        
        DESTROY(root);
    }
    void testArrayLiteral4()
    {
        PARSE_STATEMENT(L"[5,6]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral* a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a);
        CPPUNIT_ASSERT_EQUAL(2, a->numElements());
        
        DESTROY(root);
    }
    void testArrayLiteral5()
    {
        PARSE_STATEMENT(L"[5,6, [5]]");
        CPPUNIT_ASSERT(root != NULL);
        ArrayLiteral*a = dynamic_cast<ArrayLiteral*>(root);
        CPPUNIT_ASSERT(a);
        CPPUNIT_ASSERT_EQUAL(3, a->numElements());
        
        DESTROY(root);
        
    }
    void testDictionaryLiteral()
    {
        PARSE_STATEMENT(L"[:]");
        CPPUNIT_ASSERT(root != NULL);
        DictionaryLiteral* d = dynamic_cast<DictionaryLiteral*>(root);
        CPPUNIT_ASSERT(d != NULL);
        CPPUNIT_ASSERT_EQUAL(0, d->numElements());
        
        DESTROY(root);
    }
    void testDictionaryLiteral2()
    {
        PARSE_STATEMENT(L"[5 : 1]");
        CPPUNIT_ASSERT(root != NULL);
        DictionaryLiteral* d = dynamic_cast<DictionaryLiteral*>(root);
        CPPUNIT_ASSERT(d != NULL);
        CPPUNIT_ASSERT_EQUAL(1, d->numElements());
        
        DESTROY(root);
    }
    void testCompileConstants()
    {
        PARSE_STATEMENT(L"__FILE__");
        CPPUNIT_ASSERT(root != NULL);
        CompileConstant* c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"<file>", c->getValue().c_str());
        
        DESTROY(root);
    }
    void testCompileConstants2()
    {
        PARSE_STATEMENT(L"__LINE__");
        CPPUNIT_ASSERT(root != NULL);
        CompileConstant* c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"1", c->getValue().c_str());
        
        
        DESTROY(root);
    }
    void testCompileConstants3()
    {
        PARSE_STATEMENT(L"__COLUMN__");
        CPPUNIT_ASSERT(root != NULL);
        CompileConstant* c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"1", c->getValue().c_str());
        
        
        DESTROY(root);
    }
    void testCompileConstants4()
    {
        PARSE_STATEMENT(L"__FUNCTION__");
        CPPUNIT_ASSERT(root != NULL);
        CompileConstant* c = dynamic_cast<CompileConstant*>(root);
        CPPUNIT_ASSERT(c != NULL);
        ASSERT_EQUALS(L"<top>", c->getValue().c_str());
        
        DESTROY(root);
    }
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestLiteralExpression, "alltest");