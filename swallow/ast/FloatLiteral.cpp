#include "FloatLiteral.h"
#include "NodeVisitor.h"
USE_SWIFT_NS

FloatLiteral::FloatLiteral()
    :Expression(NodeType::FloatLiteral), valueAsString(L"0.0"), value(0)
{
}

void FloatLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitFloat);
}
