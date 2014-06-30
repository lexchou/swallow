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
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//STRUCT_DEF_H
