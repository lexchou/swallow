#ifndef PROTOCOL_COMPOSITION_H
#define PROTOCOL_COMPOSITION_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class TypeIdentifier;
class ProtocolComposition : public TypeNode
{
public:
    ProtocolComposition();
public:
    void addProtocol(TypeIdentifier* protocol);
    TypeIdentifier* getProtocol(int i);
    int numProtocols();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//PROTOCOL_COMPOSITION_H
