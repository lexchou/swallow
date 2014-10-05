#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H
#include "swift_conf.h"
#include "Symbol.h"
#include <vector>

SWIFT_NS_BEGIN
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::shared_ptr<class FunctionDef> FunctionDefPtr;
typedef std::weak_ptr<class FunctionDef> FunctionDefWeakPtr;
typedef std::shared_ptr<class CodeBlock> CodeBlockPtr;
typedef std::weak_ptr<class CodeBlock> CodeBlockWeakPtr;
class FunctionSymbol : public Symbol
{
    friend class FunctionOverloadedSymbol;
    friend class SymbolRegistry;
public:
    FunctionSymbol(const std::wstring& name, const TypePtr& functionType, const CodeBlockPtr& definition);
public:
    virtual const std::wstring& getName() const;
    TypePtr getReturnType()const;
    TypePtr getType();
    CodeBlockPtr getDefinition();
private:
    std::wstring name;
    TypePtr type;
    CodeBlockWeakPtr definition;
};


SWIFT_NS_END
#endif//FUNCTION_SYMBOL_H
