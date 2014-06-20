#include "identifier.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :Node(0), identifier(identifier)
{
}
