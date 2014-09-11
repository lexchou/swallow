#include "../utils.h"
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "swift_errors.h"


using namespace Swift;

TEST(TestGeneric, GenericParameter)
{
    SEMANTIC_ANALYZE(L"func swapTwoValues<T>(inout a: T, inout b: T) {\n"
            "    let temporaryA = a\n"
            "    a = b\n"
            "    b = temporaryA\n"
            "}\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


}