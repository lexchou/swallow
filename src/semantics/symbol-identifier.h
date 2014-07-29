#ifndef SYMBOL_IDENTIFIER_H
#define SYMBOL_IDENTIFIER_H
#include "ast/identifier.h"
#include "semantics/symbol.h"

SWIFT_NS_BEGIN

class SymbolIdentifier : public Identifier, public Symbol
{
public:
    virtual const std::wstring& getName() const {return identifier;}
};



SWIFT_NS_END


#endif//SYMBOL_IDENTIFIER_H