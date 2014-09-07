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
    enum Role
    {
        R_LOCAL_VARIABLE,
        R_PARAMETER,
        R_UPVALUE,
        R_PROPERTY
    };
    enum Flags
    {
        F_INITIALIZING = 1,
        F_INITIALIZED = 2,
        F_MEMBER= 4,
        F_WRITABLE = 8,
        F_READABLE = 0x10


    };
public:
    SymbolPlaceHolder(const std::wstring& name, const TypePtr& type, Role role, int flags)
    :name(name), type(type), role(role), flags(flags){}
public:
    virtual const std::wstring& getName() const override {return name;}
    virtual TypePtr getType() override {return type;}
    void setType(const TypePtr& type) {this->type = type;}
    Role getRole()const {return role;}
private:
    std::wstring name;
    TypePtr type;
    Role role;
public:
    int flags;

};

SWIFT_NS_END


#endif//SYMBOL_H