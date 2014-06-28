#ifndef CLASS_DEF_H
#define CLASS_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class ClassDef : public Declaration
{
public:
    ClassDef();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//CLASS_DEF_H
