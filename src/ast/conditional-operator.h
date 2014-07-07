#ifndef CONDITIONAL_OPERATOR_H
#define CONDITIONAL_OPERATOR_H
#include "operator.h"
#include <string>

SWIFT_NS_BEGIN
class ConditionalOperator : public Operator
{
public:
    ConditionalOperator(Pattern* expr, Expression* trueExpr, Expression* falseExpr);
    ~ConditionalOperator();
public:
    Pattern* getCondition() { return condition;}
    Expression* getTrueExpression() { return trueExpression;}
    Expression* getFalseExpression() { return falseExpression;}
    
    
    void setCondition(Pattern* v) { condition = v;}
    void setTrueExpression(Expression* v) { trueExpression = v;}
    void setFalseExpression(Expression* v) { falseExpression = v;}
public:
    
    virtual int numChildren();
    virtual Node* get(int i);
    virtual void set(int i, Node* val);
public:
    virtual void serialize(std::wostream& out);
private:
    Pattern* condition;
    Expression* trueExpression;
    Expression* falseExpression;
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
