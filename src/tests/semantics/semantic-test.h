#ifndef SEMANTIC_TEST_H
#define SEMANTIC_TEST_H
#include "tests/utils.h"
#include "semantics/symbol-resolve-action.h"
#include "semantics/type-inference-action.h"
#include "semantics/scoped-node-factory.h"
#include "semantics/scoped-nodes.h"
#include "semantics/symbol-registry.h"

static Swift::ScopedProgramPtr analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;
    registry.getCurrentScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(L"println", nullptr)));
    ScopedNodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(str));
    //ScopedProgram* f;
    dumpCompilerResults(compilerResults);
    //d.g();
    if(!ret)
        return ret;
    try
    {
        SymbolResolveAction symbolResolve(&registry, &compilerResults);
        TypeInferenceAction typeInference(&registry, &compilerResults);
        NodeVisitor* visitors[] = {&symbolResolve, &typeInference};
        for(NodeVisitor* visitor : visitors)
        {
            ret->accept(visitor);
            if(compilerResults.numResults() > 0)
                break;
        }
    }
    catch(const Abort&)
    {
        //ignore this
    }

    return ret;
}

#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::SymbolRegistry symbolRegistry; \
    Swift::CompilerResults compilerResults; \
    ScopedProgramPtr root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    Swift::SymbolScope* scope = root->getScope(); \
    (void)scope;




#endif//SEMANTIC_TEST_H
