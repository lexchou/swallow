#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class Identifier : public Node
{
public:
    Identifier(const std::wstring& identifier);
private:
    std::wstring identifier;
};

SWIFT_NS_END



#endif//IDENTIFIER_H
