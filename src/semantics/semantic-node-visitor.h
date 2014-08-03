#ifndef SEMANTIC_NODE_VISITOR_H
#define SEMANTIC_NODE_VISITOR_H
#include "ast/node-visitor.h"
#include "semantic-types.h"
#include <string>
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

    /**
     * Outputs an compiler error
     */
    void error(const NodePtr& node, int code, const std::wstring& item = std::wstring());

protected:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
};

SWIFT_NS_END


#endif//SEMANTIC_NODE_VISITOR_H