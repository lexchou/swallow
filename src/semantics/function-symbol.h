#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H
#include "swift_conf.h"
#include "symbol.h"
#include <vector>

SWIFT_NS_BEGIN
typedef std::shared_ptr<Symbol> SymbolPtr;
class FunctionSymbol : public Symbol
{
    friend class FunctionOverloadedSymbol;
    friend class SymbolRegistry;
public:
    FunctionSymbol(const std::wstring& name);
public:
    virtual const std::wstring& getName() const;
    const TypePtr& getReturnType()const;
private:
    std::wstring name;
    TypePtr returnType;
    std::vector<TypePtr> parameterTypes;

};


SWIFT_NS_END
#endif//FUNCTION_SYMBOL_H