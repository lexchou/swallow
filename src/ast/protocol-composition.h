#ifndef PROTOCOL_COMPOSITION_H
#define PROTOCOL_COMPOSITION_H
#include "type-node.h"
#include <string>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class TypeIdentifier;
class ProtocolComposition : public TypeNode
{
public:
    ProtocolComposition();
    ~ProtocolComposition();
public:
    void addProtocol(TypeIdentifierPtr protocol);
    TypeIdentifierPtr getProtocol(int i);
    int numProtocols();
private:
    std::vector<TypeIdentifierPtr> protocols;
};

SWIFT_NS_END

#endif//PROTOCOL_COMPOSITION_H
