#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H
#include "type-declaration.h"
#include <string>

SWIFT_NS_BEGIN

class StructDef : public TypeDeclaration
{
public:
    StructDef();
public:
    using Declaration::setGenericParameters;
    using Declaration::getGenericParameters;
public:
    virtual void accept(NodeVisitor* visitor);
};

SWIFT_NS_END

#endif//STRUCT_DEF_H
