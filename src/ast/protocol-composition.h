#ifndef PROTOCOL_COMPOSITION_H
#define PROTOCOL_COMPOSITION_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class ProtocolComposition : public TypeNode
{
public:
    ProtocolComposition();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//PROTOCOL_COMPOSITION_H
