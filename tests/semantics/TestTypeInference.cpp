#include "../utils.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
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
    SymbolPtr a = NULL;

    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == String);
}

TEST(TestTypeInference, testTupleLiteral)
{
    SEMANTIC_ANALYZE(L"let a = (3, true, \"str\")");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType(), t_Int, t_Bool, t_String;
    ASSERT_NOT_NULL(type);
    ASSERT_EQ(Type::Tuple, type->getCategory());
    ASSERT_EQ(3, type->numElementTypes());

    t_Int = symbolRegistry.lookupType(L"Int");
    t_Bool = symbolRegistry.lookupType(L"Bool");
    t_String = symbolRegistry.lookupType(L"String");

    ASSERT_TRUE(t_Int == type->getElementType(0));
    ASSERT_TRUE(t_Bool == type->getElementType(1));
    ASSERT_TRUE(t_String == type->getElementType(2));
}


TEST(TestTypeInference, testNamedTupleLiteral)
{
    //TODO: parse  let a = (a : 3, b : 4, c : 5)
}



TEST(TestTypeInference, testArrayLiteral)
{
    SEMANTIC_ANALYZE(L"let a = [1, 2]");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType(), t_Int, innerType;
    t_Int = symbolRegistry.lookupType(L"Int");

    ASSERT_NOT_NULL(type);
    ASSERT_EQ(Type::Array, type->getCategory());

    ASSERT_NOT_NULL(innerType = type->getInnerType());
    ASSERT_EQ(t_Int, innerType);


}

TEST(TestTypeInference, testArrayLiteral2)
{
    SEMANTIC_ANALYZE(L"let a = [1, 3.4]");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType(), t_Double, innerType;
    t_Double = symbolRegistry.lookupType(L"Double");

    ASSERT_NOT_NULL(type);
    ASSERT_EQ(Type::Array, type->getCategory());

    ASSERT_NOT_NULL(innerType = type->getInnerType());
    ASSERT_EQ(t_Double, innerType);


}

TEST(TestTypeInference, testArrayLiteral3)
{
    SEMANTIC_ANALYZE(L"let a : Int8[] = [1, 3]");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType(), Int8, innerType;
    Int8 = symbolRegistry.lookupType(L"Int8");

    ASSERT_NOT_NULL(type);
    ASSERT_EQ(Type::Array, type->getCategory());

    ASSERT_NOT_NULL(innerType = type->getInnerType());
    ASSERT_EQ(Int8, innerType);
}

TEST(TestTypeInference, testArrayLiteral4)
{
/*
    SEMANTIC_ANALYZE(L"class Base {}  "
            "class Base2 : Base{}  "
            "class Child1 : Base2{}  "
            "class Child2 : Base2{}"
            "let a : Int8[] = [1, 3]");
    SymbolPtr a = NULL;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType(), innerType;
    TypePtr t_Base2 = std::dynamic_pointer_cast<Type>(scope->lookup(L"Base2"));
    ASSERT_NOT_NULL(t_Base2);
    ASSERT_NOT_NULL(type);
    ASSERT_EQ(Type::Array, type->getCategory());

    ASSERT_NOT_NULL(innerType = type->getInnerType());
    ASSERT_EQ(t_Base2, innerType);
    */
}

//TODO: Test ArrayLiteralConvertible protocol
//TODO: Test array literal errors



TEST(TestTypeInference, testDictionaryLiteral)
{

}


TEST(TestTypeInference, testTupleAssignment)
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


    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
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


    SymbolPtr t, a;
    ASSERT_NOT_NULL(t = scope->lookup(L"t"));
    TypePtr type = t->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Test;
    ASSERT_NOT_NULL(t_Test = std::dynamic_pointer_cast<Type>(scope->lookup(L"Test")));
    ASSERT_TRUE(type == t_Test);

    ASSERT_NOT_NULL(t_Test->getReference());
    ScopedStructPtr def;
    ASSERT_NOT_NULL(def = std::dynamic_pointer_cast<ScopedStruct>(t_Test->getReference()));


    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(type = a->getType());
    TypePtr t_Int;
    ASSERT_NOT_NULL(t_Int = std::dynamic_pointer_cast<Type>(symbolRegistry.lookupType(L"Int")));
    ASSERT_TRUE(type == t_Int);



}


