#ifndef INTEGER_LITERAL_H
#define INTEGER_LITERAL_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class IntegerLiteral : public Expression
{
public:
    IntegerLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};


SWIFT_NS_END



#endif//INTEGER_LITERAL_H
