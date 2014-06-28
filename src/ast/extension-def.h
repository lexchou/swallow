#ifndef EXTENSION_DEF_H
#define EXTENSION_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class ExtensionDef : public Declaration
{
public:
    ExtensionDef();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//EXTENSION_DEF_H
