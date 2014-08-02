#ifndef TUPLE_TYPE_H
#define TUPLE_TYPE_H
#include "type-node.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN
class Attribute;
class TupleType : public TypeNode
{
public:
    struct TupleElement
    {
        bool inout;
        std::wstring name;
        TypeNodePtr type;
        TupleElement(bool inout, const std::wstring& name, const TypeNodePtr& type)
        :inout(inout), name(name), type(type){}
    };
public:
    TupleType();
    ~TupleType();
public:
    void add(bool inout, const std::wstring& name, const TypeNodePtr& type);
    int numElements();
    TypeNodePtr getElementType(int i);
    const TupleElement& getElement(int i);
    
    void setVariadicParameters(bool val);
    bool getVariadicParameters()const;

    std::vector<TupleElement>::iterator begin() {return elements.begin();};
    std::vector<TupleElement>::iterator end() {return elements.end();};

public:
    virtual void serialize(std::wostream& out);
private:
    bool variadicParameters;
    std::vector<TupleElement> elements;
};

SWIFT_NS_END

#endif//TUPLE_TYPE_H
