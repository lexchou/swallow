#ifndef SEMANTIC_NODE_VISITOR_H
#define SEMANTIC_NODE_VISITOR_H
#include "ast/node-visitor.h"
#include "semantic-types.h"
SWIFT_NS_BEGIN
class CompilerResults;
class SemanticNodeVisitor : public NodeVisitor
{
protected:
    SemanticNodeVisitor(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
            :symbolRegistry(symbolRegistry), compilerResults(compilerResults)
    {}
public:
    SymbolRegistry* getSymbolRegistry() { return symbolRegistry;}
protected:
    /**
     * Abort the visitor
     */
    void abort();
protected:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
};

SWIFT_NS_END


#endif//SEMANTIC_NODE_VISITOR_H