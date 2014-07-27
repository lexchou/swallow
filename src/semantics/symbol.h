#ifndef SYMBOL_H
#define SYMBOL_H
#include "swift_conf.h"
#include <string>

SWIFT_NS_BEGIN

class Symbol
{
public:
    Symbol(){}
    virtual ~Symbol(){}
public:
    virtual const std::wstring& getName() const = 0;

};
class SymbolPlaceHolder : public Symbol
{
public:
    SymbolPlaceHolder(const std::wstring& name)
    :name(name){}
public:
    virtual const std::wstring& getName() const {return name;}
private:
    std::wstring name;

};

SWIFT_NS_END


#endif//SYMBOL_H