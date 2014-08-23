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
    enum Flags
    {
        F_INITIALIZING = 1,
        F_INITIALIZED = 2,
    };
public:
    SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, int flags = 0)
    :name(name), type(type), flags(flags){}
public:
    virtual const std::wstring& getName() const override {return name;}
    virtual TypePtr getType() override {return type;}
    void setType(const TypePtr& type) {this->type = type;}
private:
    std::wstring name;
    TypePtr type;
public:
    int flags;

};

SWIFT_NS_END


#endif//SYMBOL_H