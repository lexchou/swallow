#ifndef SYMBOL_H
#define SYMBOL_H
#include "swift_conf.h"
#include <string>

SWIFT_NS_BEGIN
typedef std::shared_ptr<class Type> TypePtr;
class Symbol
{
public:
    Symbol(){}
    virtual ~Symbol(){}
public:
    virtual const std::wstring& getName() const = 0;
    virtual TypePtr getType() {return nullptr;}
};
class SymbolPlaceHolder : public Symbol
{
public:
    SymbolPlaceHolder(const std::wstring& name, const TypePtr& type)
    :name(name), type(type){}
public:
    virtual const std::wstring& getName() const {return name;}
    virtual TypePtr getType() {return type;}
private:
    std::wstring name;
    TypePtr type;

};

SWIFT_NS_END


#endif//SYMBOL_H