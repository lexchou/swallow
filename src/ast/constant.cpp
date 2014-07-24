#include <ast/node-visitor.h>
#include "constant.h"
#include "pattern.h"
#include "expression.h"
USE_SWIFT_NS


Constant::Constant()
    :Declaration(NodeType::Constant), name(NULL), initializer(NULL)
{
}
Constant::~Constant()
{
    SafeDelete(name);
    SafeDelete(initializer);

}
void Constant::serialize(std::wostream& out)
{
}

void Constant::accept(NodeVisitor* visitor)
{
    visitor->visitConstant(this);
}