#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "operator.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public Operator
{
public:
    BinaryOperator(const std::wstring& op, Associativity::T associativity, int precedence);
    ~BinaryOperator();
public:
    void setLHS(Pattern* val){lhs = val;}
    Pattern* getLHS(){return lhs;}

    void setRHS(Pattern* val){rhs = val;}
    Pattern* getRHS(){return rhs;}
    
    const std::wstring& getOperator() const { return op;}
public:
    
    virtual int numChildren();
    virtual Node* get(int i);
    virtual void set(int i, Node* val);
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring op;
    Pattern* lhs;
    Pattern* rhs;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
