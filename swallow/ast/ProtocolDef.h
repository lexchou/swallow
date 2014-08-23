#ifndef PROTOCOL_DEF_H
#define PROTOCOL_DEF_H
#include "TypeDeclaration.h"
#include <string>

SWIFT_NS_BEGIN

class ProtocolDef : public TypeDeclaration
{
public:
    ProtocolDef();
public:
    virtual void accept(NodeVisitor* visitor);
};

SWIFT_NS_END

#endif//PROTOCOL_DEF_H
