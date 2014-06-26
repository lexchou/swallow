#ifndef CONDITIONAL_OPERATOR_H
#define CONDITIONAL_OPERATOR_H
#include "operator-node.h"
#include <string>

SWIFT_NS_BEGIN
class ConditionalOperator : public OperatorNode
{
public:
    ConditionalOperator(ExpressionNode* expr, ExpressionNode* trueExpr, ExpressionNode* falseExpr);
public:
    ExpressionNode* getCondition() { return static_cast<ExpressionNode*>(get(0));}
    ExpressionNode* getTrueExpression() { return static_cast<ExpressionNode*>(get(2));}
    ExpressionNode* getFalseExpression() { return static_cast<ExpressionNode*>(get(1));}
    
    
    void setCondition(Statement* v) { set(0, v);}
    void setTrueExpression(ExpressionNode* v) { set(2, v);}
    void setFalseExpression(ExpressionNode* v) { set(1, v);}
    
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END



#endif//TYPE_CHECK_H
