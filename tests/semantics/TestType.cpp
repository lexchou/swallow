#include "../utils.h"
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "swift_errors.h"


using namespace Swift;

TEST(TestType, testCovariance)
{
    TypePtr Base = Type::newType(L"Base", Type::Class);
    TypePtr Base2 = Type::newType(L"Base2", Type::Class, nullptr, Base);

    TypePtr Child1 = Type::newType(L"Child1", Type::Class, nullptr, Base2);
    TypePtr Child2 = Type::newType(L"Child2", Type::Class, nullptr, Base2);
    //calculate the common parent class between Child1 and Child2 with the minimum inheritance distance
    TypePtr parent = Child1->getCommonParent(Child2);
    ASSERT_NOT_NULL(parent);
    ASSERT_TRUE(parent == Base2);

}

TEST(TestType, testInheritance)
{
    SEMANTIC_ANALYZE(L"class SomeSuperclass{}"
            "class SomeClass : SomeSuperclass{}");
    TypePtr SomeSuperclass, SomeClass;

    ASSERT_NOT_NULL(SomeSuperclass = std::dynamic_pointer_cast<Type>(scope->lookup(L"SomeSuperclass")));
    ASSERT_NOT_NULL(SomeClass = std::dynamic_pointer_cast<Type>(scope->lookup(L"SomeClass")));

    ASSERT_EQ(SomeSuperclass, SomeClass->getParentType());

}

TEST(TtestType, testTypealias)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Int");
    TypePtr IntegerLiteralType, Int;

    Int = symbolRegistry.lookupType(L"Int");
    ASSERT_NOT_NULL(IntegerLiteralType = std::dynamic_pointer_cast<Type>(scope->lookup(L"IntegerLiteralType")));
    ASSERT_EQ(Int, IntegerLiteralType);
}


TEST(TtestType, testTypealias_UndeclaredType)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Intff");

    ASSERT_EQ(1, compilerResults.numResults());
    auto result = compilerResults.getResult(0);

    ASSERT_EQ(Errors::E_USE_OF_UNDECLARED_TYPE_1, result.code);
    ASSERT_EQ(L"Intff", result.items[0]);
}

TEST(TtestType, testTypealias_Redefinition)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Int\n"
            "typealias IntegerLiteralType = Intff");

    ASSERT_EQ(1, compilerResults.numResults());
    auto result = compilerResults.getResult(0);

    ASSERT_EQ(Errors::E_INVALID_REDECLARATION_1, result.code);
    ASSERT_EQ(L"IntegerLiteralType", result.items[0]);
}