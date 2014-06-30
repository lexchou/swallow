#ifndef PROTOCOL_DEF_H
#define PROTOCOL_DEF_H
#include "type-declaration.h"
#include <string>

SWIFT_NS_BEGIN

class ProtocolDef : public TypeDeclaration
{
public:
    ProtocolDef();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//PROTOCOL_DEF_H
