#include "../utils.h"

using namespace Swift;

TEST(TestEnumeration, testEnum)
{
    PARSE_STATEMENT(L"enum SomeEnumeration {"
                       L"}");
    EnumDefPtr e;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(L"SomeEnumeration", std::static_pointer_cast<TypeIdentifier>(e->getIdentifier())->getName());
    ASSERT_EQ(0, e->numConstants());
    ASSERT_EQ(0, e->numAssociatedTypes());
}

TEST(TestEnumeration, testEnum2)
{
    PARSE_STATEMENT(L"enum CompassPoint {"
                       L"case North "
                       L"case South "
                       L"case East "
                       L"case West "
                       L"}");
    EnumDefPtr e;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(4, e->numAssociatedTypes());
    ASSERT_EQ(L"North", e->getAssociatedType(0).name);
    ASSERT_EQ(L"South", e->getAssociatedType(1).name);
    ASSERT_EQ(L"East", e->getAssociatedType(2).name);
    ASSERT_EQ(L"West", e->getAssociatedType(3).name);

}
TEST(TestEnumeration, testEnum3)
{
    PARSE_STATEMENT(L"enum Planet {"
                       L"case Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune"
                       L"}");
    EnumDefPtr e;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(8, e->numAssociatedTypes());
    ASSERT_EQ(L"Mercury", e->getAssociatedType(0).name);
    ASSERT_EQ(L"Venus", e->getAssociatedType(1).name);
    ASSERT_EQ(L"Earth", e->getAssociatedType(2).name);
    ASSERT_EQ(L"Mars", e->getAssociatedType(3).name);

}

TEST(TestEnumeration, testAssociatedValues)
{
    PARSE_STATEMENT(L"enum Barcode {"
                       L"case UPCA(Int, Int, Int)"
                       L"case QRCode(String)"
                       L"}");
    EnumDefPtr e;
    TupleTypePtr tuple;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(2, e->numAssociatedTypes());

    ASSERT_EQ(L"UPCA", e->getAssociatedType(0).name);
    ASSERT_NOT_NULL(tuple = e->getAssociatedType(0).value);
    ASSERT_EQ(3, tuple->numElements());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(0)));
    ASSERT_EQ(L"Int", t->getName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(1)));
    ASSERT_EQ(L"Int", t->getName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(2)));
    ASSERT_EQ(L"Int", t->getName());

    ASSERT_EQ(L"QRCode", e->getAssociatedType(1).name);
    ASSERT_NOT_NULL(tuple = e->getAssociatedType(1).value);
    ASSERT_EQ(1, tuple->numElements());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(tuple->getElementType(0)));
    ASSERT_EQ(L"String", t->getName());

}
TEST(TestEnumeration, testRawValues)
{
    PARSE_STATEMENT(L"enum ASCIIControlCharacter: Character {"
                       L"case Tab = \"\\t\""
                       L"case LineFeed = \"\\n\""
                       L"case CarriageReturn = \"\\r\""
                       L"}");
    EnumDefPtr e;
    StringLiteralPtr s;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(3, e->numConstants());
    ASSERT_EQ(1, e->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier()));
    ASSERT_EQ(L"ASCIIControlCharacter", t->getName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getParent(0)));
    ASSERT_EQ(L"Character", t->getName());

    ASSERT_EQ(L"Tab", e->getConstant(0).name);
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(0).value));
    ASSERT_EQ(L"\t", s->toString());

    ASSERT_EQ(L"LineFeed", e->getConstant(1).name);
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(1).value));
    ASSERT_EQ(L"\n", s->toString());

    ASSERT_EQ(L"CarriageReturn", e->getConstant(2).name);
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StringLiteral>(e->getConstant(2).value));
    ASSERT_EQ(L"\r", s->toString());

}

TEST(TestEnumeration, testRawValues2)
{
    PARSE_STATEMENT(L"enum Planet: Int {"
                       L"case Mercury = 1, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune"
                       L"}");

    EnumDefPtr e;
    TypeIdentifierPtr t;
    IntegerLiteralPtr i;
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(8, e->numConstants());
    ASSERT_EQ(1, e->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getIdentifier()));
    ASSERT_EQ(L"Planet", t->getName());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(e->getParent(0)));
    ASSERT_EQ(L"Int", t->getName());

    ASSERT_EQ(L"Mercury", e->getConstant(0).name);
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(e->getConstant(0).value));
    ASSERT_EQ(L"1", i->valueAsString);

}




