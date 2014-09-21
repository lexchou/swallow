#include "../utils.h"
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "swift_errors.h"


using namespace Swift;

TEST(TestGeneric, Func_GenericParameter)
{
    SEMANTIC_ANALYZE(L"func swapTwoValues<T>(inout a: T, inout b: T) {\n"
            "    let temporaryA = a\n"
            "    a = b\n"
            "    b = temporaryA\n"
            "}\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


}
TEST(TestGeneric, Struct_GenericParameter)
{
    SEMANTIC_ANALYZE(L"struct Stack<T> {\n"
            "    var items : T\n"
            "    mutating func push(item: T) {\n"
            "        //items.append(item)\n"
            "    }\n"
            "    mutating func pop() -> T {\n"
            "        //return items.removeLast()\n"
            "    }\n"
            "}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestGeneric, Use_GenericType)
{
    SEMANTIC_ANALYZE(L"struct Stack<T> {}\n"
            "let stackOfInts : Stack<Int>");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPlaceHolderPtr stackOfInts;
    TypePtr type, Stack, Int;
    ASSERT_NOT_NULL(stackOfInts = std::dynamic_pointer_cast<SymbolPlaceHolder>(scope->lookup(L"stackOfInts")));
    ASSERT_NOT_NULL(Stack = std::dynamic_pointer_cast<Type>(scope->lookup(L"Stack")));
    ASSERT_NOT_NULL(Int = symbolRegistry.lookupType(L"Int"));
    ASSERT_NOT_NULL(type = stackOfInts->getType());
    ASSERT_EQ(Type::Specialized, type->getCategory());
    ASSERT_EQ(Stack, type->getInnerType());
    ASSERT_EQ(1, type->getGenericArguments().size());
    ASSERT_EQ(Int, type->getGenericArguments()[0]);


}

TEST(TestGeneric, GenericConstraint)
{
    SEMANTIC_ANALYZE(L"func findIndex<T: Equatable>(array: Array<T>, valueToFind: T) -> Int? {\n"
            //"    for (index, value) in enumerate(array) {\n"
            //"        if value == valueToFind {\n"
            //"            return index\n"
            //"        }\n"
            //"    }\n"
            //"    return nil\n"
            "}\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestGeneric, GenericConstraint2)
{
    SEMANTIC_ANALYZE(L"class Base {}\n"
            "func test<T : Base>(a : T)\n"
            "{\n"
            "}\n"
            "var a : Int = 3\n"
            "test(\"\");");
    ASSERT_EQ(1, compilerResults.numResults());
}