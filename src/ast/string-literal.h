#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class StringLiteral : public Expression
{
public:
    StringLiteral();
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
public:
    std::wstring value;
};

SWIFT_NS_END



#endif//STRING_LITERAL_H
