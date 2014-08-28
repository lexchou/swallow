#include "../utils.h"
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "swift_errors.h"


using namespace Swift;

TEST(TestProtocol, testProtocolInheritance)
{
    SEMANTIC_ANALYZE(L"protocol Base{}\n"
            "protocol Child : Base{}")
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr Base, Child;
    ASSERT_NOT_NULL(Base = std::dynamic_pointer_cast<Type>(scope->lookup(L"Base")));
    ASSERT_NOT_NULL(Child = std::dynamic_pointer_cast<Type>(scope->lookup(L"Child")));

}

TEST(TestProtocol, testMethodRequirements)
{
    SEMANTIC_ANALYZE(L"protocol RandomNumberGenerator {\n"
            L" func random() -> Double\n"
            L"  } ");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr RandomNumberGenerator;
    ASSERT_NOT_NULL(RandomNumberGenerator = std::dynamic_pointer_cast<Type>(scope->lookup(L"RandomNumberGenerator")));
    SymbolPtr sym = RandomNumberGenerator->getSymbols()[L"random"];
    FunctionSymbolPtr random;

    ASSERT_NOT_NULL(random = std::dynamic_pointer_cast<FunctionSymbol>(sym));

}
