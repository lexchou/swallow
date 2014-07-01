#ifndef ENUM_DEF_H
#define ENUM_DEF_H
#include "type-declaration.h"
#include <string>

SWIFT_NS_BEGIN
class Expression;
class TupleType;
class EnumDef : public TypeDeclaration
{
public:
    enum Type
    {
        UnionStyle,
        RawValue
    };
public:
    template<class ValueType>
    struct KeyValue
    {
        std::wstring name;
        ValueType value;
        KeyValue(const std::wstring& name, ValueType value)
        :name(name), value(value){}
    };
    typedef KeyValue<TupleType*> AssociatedType;
    typedef KeyValue<Expression*> Constant;
public:
    EnumDef();
public:
    Type getType()const;
    
    void addAssociatedType(const std::wstring& name, TupleType* associatedType);
    int numAssociatedTypes()const;
    const AssociatedType& getAssociatedType(int i);
    
    void addConstant(const std::wstring& name, Expression* value);
    int numConstants();
    const Constant& getConstant(int i);
    
public:
    virtual void serialize(std::wostream& out);
private:
    std::vector<AssociatedType> associatedTypes;
    std::vector<Constant> constants;
};

SWIFT_NS_END

#endif//ENUM_DEF_H
