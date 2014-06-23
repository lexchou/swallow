#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class StringLiteral : public ExpressionNode
{
public:
    StringLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};

SWIFT_NS_END



#endif//STRING_LITERAL_H
