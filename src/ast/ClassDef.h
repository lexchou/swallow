#ifndef CLASS_DEF_H
#define CLASS_DEF_H
#include "TypeDeclaration.h"
#include <string>

SWIFT_NS_BEGIN

class ClassDef : public TypeDeclaration
{
public:
    ClassDef();
public:
    using Declaration::setGenericParameters;
    using Declaration::getGenericParameters;
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//CLASS_DEF_H
