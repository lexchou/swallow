#ifndef PROTOCOL_DEF_H
#define PROTOCOL_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class ProtocolDef : public Declaration
{
public:
    ProtocolDef();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//PROTOCOL_DEF_H
