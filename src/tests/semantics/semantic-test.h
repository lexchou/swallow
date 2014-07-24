#ifndef SEMANTIC_TEST_H
#define SEMANTIC_TEST_H
#include "tests/utils.h"
#include "semantics/symbol-resolve-action.h"
#include "semantics/type-inference-action.h"
#include "semantics/scoped-node-factory.h"
#include "semantics/scoped-nodes.h"

class SemanticTestCase : public SwiftTestCase
{
public:
    Swift::ScopedProgram* analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
    {
        using namespace Swift;

        ScopedNodeFactory nodeFactory;
        Parser parser(&nodeFactory, &registry, &compilerResults);
        parser.setFileName(L"<file>");
        ScopedProgram* ret = dynamic_cast<ScopedProgram*>(parser.parse(str));
        dumpCompilerResults(compilerResults);

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
    ScopedProgram* root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    tracer.node = root; \
    Swift::SymbolScope* scope = root->getScope(); \
    (void)scope;




#endif//SEMANTIC_TEST_H
