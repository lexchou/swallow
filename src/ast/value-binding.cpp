#include <ast/node-visitor.h>
#include "constant.h"
#include "pattern.h"
#include "expression.h"
USE_SWIFT_NS


const TypePtr& ValueBinding::getType()const
{
    return type;
}

void ValueBinding::setType(const TypePtr& type)
{
    this->type = type;
}

const PatternPtr& ValueBinding::getName()const
{
    return name;
}
void ValueBinding::setName(const PatternPtr& name)
{
    this->name = name;
}

void ValueBinding::setInitializer(const ExpressionPtr& initializer)
{
    this->initializer = initializer;
}
const ExpressionPtr& ValueBinding::getInitializer()const
{
    return initializer;
}
