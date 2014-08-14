#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/semantics/semantic-test.h"
#include "semantics/type.h"
#include "swift_errors.h"


using namespace Swift;

TEST(TestTypeInference, testIntLiteral)
{
    SEMANTIC_ANALYZE(L"let a = 34");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr Int = symbolRegistry.lookupType(L"Int");
    ASSERT_TRUE(type == Int);

}
TEST(TestTypeInference, testStringLiteral)
{
    SEMANTIC_ANALYZE(L"let a = \"345\"");
    IdentifierPtr a = NULL;

    ASSERT_NOT_NULL(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == String);
}
TEST(TestTypeInference, testTuple)
{
    /*
    SEMANTIC_ANALYZE(L"let (a, b) : Int = (1, 0.3)");
    IdentifierPtr a = NULL;
    ASSERT_TRUE(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
    TypePtr type = a->getType();
    ASSERT_TRUE(type);
    TypePtr String = symbolRegistry.lookupType(L"Int");
    ASSERT_TRUE(type == String);
    */
}
TEST(TestTypeInference, testExpression)
{
    SEMANTIC_ANALYZE(L"let a = 3 * 4 + 3");
    ASSERT_EQ(0, compilerResults.numResults());


    IdentifierPtr a;
    ASSERT_NOT_NULL(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_int = symbolRegistry.lookupType(L"Int");
    ASSERT_TRUE(type == t_int);
}

TEST(TestTypeInference, testStructInstance)
{

    SEMANTIC_ANALYZE(L"struct Test {let a = 4}\n"
    L"let t = Test()\n"
    L"let a = t.a"
    );
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


    IdentifierPtr t, a;
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"t")));
    TypePtr type = t->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Test;
    ASSERT_NOT_NULL(t_Test = std::dynamic_pointer_cast<Type>(scope->lookup(L"Test")));
    ASSERT_TRUE(type == t_Test);

    ASSERT_NOT_NULL(t_Test->getReference());
    ScopedStructPtr def;
    ASSERT_NOT_NULL(def = std::dynamic_pointer_cast<ScopedStruct>(t_Test->getReference()));


    ASSERT_NOT_NULL(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
    ASSERT_NOT_NULL(type = a->getType());
    TypePtr t_Int;
    ASSERT_NOT_NULL(t_Int = std::dynamic_pointer_cast<Type>(symbolRegistry.lookupType(L"Int")));
    ASSERT_TRUE(type == t_Int);



}





#endif//TEST_TYPE_INFERENCE_H
