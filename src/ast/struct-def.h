#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class StructDef : public Declaration
{
public:
    StructDef();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//STRUCT_DEF_H
