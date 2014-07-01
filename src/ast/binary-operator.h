#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "operator.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public Operator
{
public:
    BinaryOperator(const std::wstring& op, Associativity::T associativity, int precedence);
public:
    void setLHS(Pattern* node){set(0, node);}
    Pattern* getLHS(){return static_cast<Pattern*>(get(0));}

    void setRHS(Pattern* node){set(1, node);}
    Pattern* getRHS(){return static_cast<Pattern*>(get(1));}
    
    const std::wstring& getOperator() const { return op;}
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring op;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
