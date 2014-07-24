#ifndef SEMANTIC_NODE_VISITOR_H
#define SEMANTIC_NODE_VISITOR_H
#include "ast/node-visitor.h"

SWIFT_NS_BEGIN
class SymbolRegistry;
class SemanticNodeVisitor : public NodeVisitor
{
protected:
    SemanticNodeVisitor(SymbolRegistry* symbolRegistry)
            :symbolRegistry(symbolRegistry)
    {}
public:
    SymbolRegistry* getSymbolRegistry() { return symbolRegistry;}
protected:
    SymbolRegistry* symbolRegistry;
};

SWIFT_NS_END


#endif//SEMANTIC_NODE_VISITOR_H