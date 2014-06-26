#ifndef TUPLE_TYPE_H
#define TUPLE_TYPE_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class TupleType : public TypeNode
{
public:
    TupleType();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//TUPLE_TYPE_H
