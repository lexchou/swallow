#ifndef FLOAT_LITERAL_H
#define FLOAT_LITERAL_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class FloatLiteral : public ExpressionNode
{
public:
    FloatLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};

SWIFT_NS_END



#endif//FLOAT_LITERAL_H
