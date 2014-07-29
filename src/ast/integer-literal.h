#ifndef INTEGER_LITERAL_H
#define INTEGER_LITERAL_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class IntegerLiteral : public Expression
{
public:
    IntegerLiteral();
public:
    virtual void serialize(std::wostream& out);
public:
    std::wstring valueAsString;
    int64_t value;
};
typedef std::shared_ptr<IntegerLiteral> IntegerLiteralPtr;

SWIFT_NS_END



#endif//INTEGER_LITERAL_H
