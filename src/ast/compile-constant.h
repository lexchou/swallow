#ifndef COMPILE_CONSTANT_H
#define COMPILE_CONSTANT_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class CompileConstant : public Expression
{
public:
    CompileConstant(const std::wstring& name, const std::wstring& value);
public:
    const std::wstring& getName() const;
    const std::wstring& getValue() const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    std::wstring value;
};

SWIFT_NS_END



#endif//COMPILE_CONSTANT_H
