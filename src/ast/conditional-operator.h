#ifndef CONDITIONAL_OPERATOR_H
#define CONDITIONAL_OPERATOR_H
#include "operator.h"
#include <string>

SWIFT_NS_BEGIN
class ConditionalOperator : public Operator
{
public:
    ConditionalOperator();
    ~ConditionalOperator();
public:
    PatternPtr getCondition() { return condition;}
    ExpressionPtr getTrueExpression() { return trueExpression;}
    ExpressionPtr getFalseExpression() { return falseExpression;}
    
    
    void setCondition(PatternPtr v) { condition = v;}
    void setTrueExpression(ExpressionPtr v) { trueExpression = v;}
    void setFalseExpression(ExpressionPtr v) { falseExpression = v;}
public:
    
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
public:
    virtual void accept(NodeVisitor* visitor);
private:
    PatternPtr condition;
    ExpressionPtr trueExpression;
    ExpressionPtr falseExpression;
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
