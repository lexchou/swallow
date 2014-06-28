#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class FunctionDef : public Declaration
{
public:
    FunctionDef();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//FUNCTION_DEF_H
