#include <ast/node-visitor.h>
#include "compile-constant.h"
USE_SWIFT_NS

CompileConstant::CompileConstant()
    :Expression(NodeType::CompileConstant)
{
    
}

void CompileConstant::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitCompileConstant);
}

const std::wstring& CompileConstant::getName() const
{
    return name;
}
const std::wstring& CompileConstant::getValue() const
{
    return value;
}
void CompileConstant::setName(const std::wstring& name)
{
    this->name = name;
}
void CompileConstant::setValue(const std::wstring& value)
{
    this->value = value;
}