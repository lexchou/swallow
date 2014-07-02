#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

using namespace Swift;

class TestDeclaration : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestDeclaration);
    CPPUNIT_TEST(testLet);
    CPPUNIT_TEST(testLet_Multiple);
    CPPUNIT_TEST(testLet_Tuple);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testLet()
    {
        Node* root = parse(L"let a : Int[] = [1, 2, 3]");
        Constant* c = NULL;
        Identifier* id = NULL;
        ArrayLiteral* value = NULL;
        ArrayType* type = NULL;
        TypeIdentifier* Int = NULL;
        CPPUNIT_ASSERT(c = dynamic_cast<Constant*>(root));
        CPPUNIT_ASSERT_EQUAL(1, c->numPairs());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getPair(0).first));
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        CPPUNIT_ASSERT(type = dynamic_cast<ArrayType*>(id->getType()));
        CPPUNIT_ASSERT(Int = dynamic_cast<TypeIdentifier*>(type->getInnerType()));
        ASSERT_EQUALS(L"Int", Int->getName().c_str());
        
        CPPUNIT_ASSERT(value = dynamic_cast<ArrayLiteral*>(c->getPair(0).second));
        CPPUNIT_ASSERT_EQUAL(3, value->numChildren());
        ASSERT_EQUALS(L"1", dynamic_cast<IntegerLiteral*>(value->get(0))->toString().c_str());
        ASSERT_EQUALS(L"2", dynamic_cast<IntegerLiteral*>(value->get(1))->toString().c_str());
        ASSERT_EQUALS(L"3", dynamic_cast<IntegerLiteral*>(value->get(2))->toString().c_str());

        DESTROY(root);
    }
    
    void testLet_Multiple()
    {
        
    }
    
    void testLet_Tuple()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestDeclaration, "alltest");



#endif