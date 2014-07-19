#ifndef SEMANTIC_TEST_H
#define SEMANTIC_TEST_H
#include "tests/utils.h"
#include "semantics/symbol-resolve-action.h"
#include "semantics/type-inference-action.h"


class SemanticTestCase : public SwiftTestCase
{
public:
    Swift::Node* analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
    {
        using namespace Swift;
        Program* ret = parseStatements(registry, compilerResults, func, str);
        if(!ret)
            return ret;
        {
            SymbolResolveAction action(&registry, &compilerResults);
            ret->accept(&action);
        }

        {
            TypeInferenceAction action(&registry);
            ret->accept(&action);
        }

        return ret;
    }
};

#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::SymbolRegistry symbolRegistry; \
    Swift::CompilerResults compilerResults; \
    Node* root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    tracer.node = root; \
    Swift::SymbolScope* scope = symbolRegistry.getCurrentScope(); \
    (void)scope;




#endif//SEMANTIC_TEST_H
