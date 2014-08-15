#include "type-alias.h"
#include "node-visitor.h"
USE_SWIFT_NS


TypeAlias::TypeAlias()
    :Declaration(NodeType::TypeAlias), type(NULL)
{
}
TypeAlias::~TypeAlias()
{
}

void TypeAlias::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitTypeAlias);
}

const std::wstring& TypeAlias::getName()const
{
    return name;
}
void TypeAlias::setName(const std::wstring& name)
{
    this->name = name;
}

void TypeAlias::setType(TypeNodePtr type)
{
    this->type = type;
}
TypeNodePtr TypeAlias::getType()
{
    return type;
}