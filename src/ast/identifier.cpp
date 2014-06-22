#include "identifier.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :ExpressionNode(0), identifier(identifier)
{
}

void Identifier::serialize(std::wostream& out)
{
    out<<identifier;
}