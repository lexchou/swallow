#ifndef CONDITIONAL_OPERATOR_H
#define CONDITIONAL_OPERATOR_H
#include "operator.h"
#include <string>

SWIFT_NS_BEGIN
class ConditionalOperator : public Operator
{
public:
    ConditionalOperator(Expression* expr, Expression* trueExpr, Expression* falseExpr);
public:
    Expression* getCondition() { return static_cast<Expression*>(get(0));}
    Expression* getTrueExpression() { return static_cast<Expression*>(get(2));}
    Expression* getFalseExpression() { return static_cast<Expression*>(get(1));}
    
    
    void setCondition(Pattern* v) { set(0, v);}
    void setTrueExpression(Expression* v) { set(2, v);}
    void setFalseExpression(Expression* v) { set(1, v);}
    
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
