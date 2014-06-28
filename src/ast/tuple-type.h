#ifndef TUPLE_TYPE_H
#define TUPLE_TYPE_H
#include "type-node.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN
class Attribute;
class TupleType : public TypeNode
{
protected:
    struct TupleElementMeta
    {
        bool inout;
        std::wstring name;
        TupleElementMeta(bool inout, const std::wstring& name)
        :inout(inout), name(name){}
    };
public:
    TupleType();
public:
    void add(bool inout, const std::wstring& name, TypeNode* type);
    int numElements();
    TypeNode* getElementType(int i);
    
    
    void setVariadicParameters(bool val);
    bool getVariadicParameters()const;
public:
    virtual void serialize(std::wostream& out);
private:
    bool variadicParameters;
    std::vector<TupleElementMeta> metas;
};

SWIFT_NS_END

#endif//TUPLE_TYPE_H
