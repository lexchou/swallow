#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

class TestEnumeration : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestEnumeration);
    CPPUNIT_TEST(testEnum);
    CPPUNIT_TEST(testEnum2);
    CPPUNIT_TEST(testEnum3);
    CPPUNIT_TEST(testAssociatedValues);
    CPPUNIT_TEST(testRawValues);
    CPPUNIT_TEST(testRawValues2);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testEnum()
    {
        PARSE_STATEMENT(L"enum SomeEnumeration {"
                           L"}");
        EnumDefPtr e;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        ASSERT_EQUALS(L"SomeEnumeration", std::static_pointer_cast<TypeIdentifier>(e->getIdentifier())->getName().c_str());
        CPPUNIT_ASSERT_EQUAL(0, e->numConstants());
        CPPUNIT_ASSERT_EQUAL(0, e->numAssociatedTypes());
    }
    
    void testEnum2()
    {
        PARSE_STATEMENT(L"enum CompassPoint {"
                           L"case North "
                           L"case South "
                           L"case East "
                           L"case West "
                           L"}");
        EnumDefPtr e;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(4, e->numAssociatedTypes());
        ASSERT_EQUALS(L"North", e->getAssociatedType(0).name.c_str());
        ASSERT_EQUALS(L"South", e->getAssociatedType(1).name.c_str());
        ASSERT_EQUALS(L"East", e->getAssociatedType(2).name.c_str());
        ASSERT_EQUALS(L"West", e->getAssociatedType(3).name.c_str());
        
    }
    void testEnum3()
    {
        PARSE_STATEMENT(L"enum Planet {"
                           L"case Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune"
                           L"}");
        EnumDefPtr e;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(8, e->numAssociatedTypes());
        ASSERT_EQUALS(L"Mercury", e->getAssociatedType(0).name.c_str());
        ASSERT_EQUALS(L"Venus", e->getAssociatedType(1).name.c_str());
        ASSERT_EQUALS(L"Earth", e->getAssociatedType(2).name.c_str());
        ASSERT_EQUALS(L"Mars", e->getAssociatedType(3).name.c_str());
        
    }
    
    void testAssociatedValues()
    {
        PARSE_STATEMENT(L"enum Barcode {"
                           L"case UPCA(Int, Int, Int)"
                           L"case QRCode(String)"
                           L"}");
        EnumDefPtr e;
        TupleTypePtr tuple;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(2, e->numAssociatedTypes());
        
        ASSERT_EQUALS(L"UPCA", e->getAssociatedType(0).name.c_str());
        CPPUNIT_ASSERT(tuple = e->getAssociatedType(0).value);
        CPPUNIT_ASSERT_EQUAL(3, tuple->numElements());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(0)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(1)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(2)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
        ASSERT_EQUALS(L"QRCode", e->getAssociatedType(1).name.c_str());
        CPPUNIT_ASSERT(tuple = e->getAssociatedType(1).value);
        CPPUNIT_ASSERT_EQUAL(1, tuple->numElements());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(0)));
        ASSERT_EQUALS(L"String", t->getName().c_str());
        
    }
    void testRawValues()
    {
        PARSE_STATEMENT(L"enum ASCIIControlCharacter: Character {"
                           L"case Tab = \"\\t\""
                           L"case LineFeed = \"\\n\""
                           L"case CarriageReturn = \"\\r\""
                           L"}");
        EnumDefPtr e;
        StringLiteralPtr s;
        TypeIdentifierPtr t;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(3, e->numConstants());
        CPPUNIT_ASSERT_EQUAL(1, e->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier()));
        ASSERT_EQUALS(L"ASCIIControlCharacter", t->getName().c_str());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getParent(0)));
        ASSERT_EQUALS(L"Character", t->getName().c_str());
        
        ASSERT_EQUALS(L"Tab", e->getConstant(0).name.c_str());
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(0).value));
        ASSERT_EQUALS(L"\t", s->toString().c_str());
        
        ASSERT_EQUALS(L"LineFeed", e->getConstant(1).name.c_str());
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(1).value));
        ASSERT_EQUALS(L"\n", s->toString().c_str());
        
        ASSERT_EQUALS(L"CarriageReturn", e->getConstant(2).name.c_str());
        CPPUNIT_ASSERT(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(2).value));
        ASSERT_EQUALS(L"\r", s->toString().c_str());
        
    }
    
    void testRawValues2()
    {
        PARSE_STATEMENT(L"enum Planet: Int {"
                           L"case Mercury = 1, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune"
                           L"}");
        
        EnumDefPtr e;
        TypeIdentifierPtr t;
        IntegerLiteralPtr i;
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumDef>(root));
        CPPUNIT_ASSERT_EQUAL(8, e->numConstants());
        CPPUNIT_ASSERT_EQUAL(1, e->numParents());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier()));
        ASSERT_EQUALS(L"Planet", t->getName().c_str());
        CPPUNIT_ASSERT(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getParent(0)));
        ASSERT_EQUALS(L"Int", t->getName().c_str());
        
        ASSERT_EQUALS(L"Mercury", e->getConstant(0).name.c_str());
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(e->getConstant(0).value));
        ASSERT_EQUALS(L"1", i->valueAsString);
        
    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestEnumeration, "alltest");



#endif
