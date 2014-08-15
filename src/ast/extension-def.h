#ifndef EXTENSION_DEF_H
#define EXTENSION_DEF_H
#include "type-declaration.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;
class ExtensionDef : public TypeDeclaration
{
public:
    ExtensionDef();
public:
    virtual void accept(NodeVisitor* visitor);
public:
};

SWIFT_NS_END

#endif//EXTENSION_DEF_H
