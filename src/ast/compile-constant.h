#ifndef COMPILE_CONSTANT_H
#define COMPILE_CONSTANT_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class CompileConstant : public Expression
{
public:
    CompileConstant();
public:
    const std::wstring& getName() const;
    void setName(const std::wstring& name);
    const std::wstring& getValue() const;
    void setValue(const std::wstring& value);
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring name;
    std::wstring value;
};

SWIFT_NS_END



#endif//COMPILE_CONSTANT_H
