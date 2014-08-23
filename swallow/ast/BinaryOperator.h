#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "Operator.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public Operator
{
public:
    BinaryOperator();
    ~BinaryOperator();
public:
    void setLHS(PatternPtr val){lhs = val;}
    PatternPtr getLHS(){return lhs;}

    void setRHS(PatternPtr val){rhs = val;}
    PatternPtr getRHS(){return rhs;}
    
    const std::wstring& getOperator() const { return op;}
    void setOperator(const std::wstring& op) {this->op = op;}
public:
    
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
public:
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring op;
    PatternPtr lhs;
    PatternPtr rhs;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
