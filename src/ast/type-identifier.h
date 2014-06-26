#ifndef TYPE_IDENTIFIER_H
#define TYPE_IDENTIFIER_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeIdentifier : public TypeNode
{
public:
    TypeIdentifier();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//TYPE_IDENTIFIER_H
