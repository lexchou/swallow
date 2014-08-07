#ifndef FUNCTION_OVERLOADED_SYMBOL_H
#define FUNCTION_OVERLOADED_SYMBOL_H
#include "swift_conf.h"
#include "symbol.h"
#include <vector>

SWIFT_NS_BEGIN
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::shared_ptr<class FunctionSymbol> FunctionSymbolPtr;
class FunctionOverloadedSymbol : public Symbol
{
public:
    FunctionOverloadedSymbol(const std::wstring& name);
public:
    virtual const std::wstring& getName() const;
    FunctionSymbolPtr lookupOverload(int argc, TypePtr argv[]);
    void add(const FunctionSymbolPtr& func);

    std::vector<FunctionSymbolPtr>::iterator begin() { return functions.begin();}
    std::vector<FunctionSymbolPtr>::iterator end() { return functions.end();}
private:
    std::wstring name;
    std::vector<FunctionSymbolPtr> functions;
};


SWIFT_NS_END

#endif//FUNCTION_OVERLOADED_SYMBOL_H