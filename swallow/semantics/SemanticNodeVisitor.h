#ifndef SEMANTIC_NODE_VISITOR_H
#define SEMANTIC_NODE_VISITOR_H
#include "ast/NodeVisitor.h"
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
    void error(const NodePtr& node, int code);
    void error(const NodePtr& node, int code, const std::wstring& item);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2);


    /**
     * Outputs an compiler error
     */
    void warning(const NodePtr& node, int code, const std::wstring& item = std::wstring());


    /**
     * Convert a AST TypeNode into symboled Type
     */
    TypePtr lookupType(const TypeNodePtr& type);
    std::wstring toString(const NodePtr& node);
private:
    TypePtr lookupTypeImpl(const TypeNodePtr& type);
protected:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
};


    template<class T>
    struct StackedValueGuard
    {
        StackedValueGuard(T & value)
                :ref(value), value(value)
        {

        }
        void set(const T& val)
        {
            ref = val;
        }

        ~StackedValueGuard()
        {
            ref = value;
        }

        T& ref;
        T value;

    };


SWIFT_NS_END


#endif//SEMANTIC_NODE_VISITOR_H
