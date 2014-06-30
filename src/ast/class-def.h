#ifndef CLASS_DEF_H
#define CLASS_DEF_H
#include "type-declaration.h"
#include <string>

SWIFT_NS_BEGIN

class ClassDef : public TypeDeclaration
{
public:
    ClassDef();
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//CLASS_DEF_H
