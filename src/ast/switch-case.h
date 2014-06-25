#ifndef SWITCH_CASE_H
#define SWITCH_CASE_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ExpressionNode;
class CaseStatement;
class SwitchCase : public Statement
{
public:
    SwitchCase();
public:
    void setControlExpression(ExpressionNode* expr);
    ExpressionNode* getControlExpression();
    
    void addCase(CaseStatement* c);
    int numCases();
    CaseStatement* getCase(int idx);
    
    void setDefaultCase(CaseStatement* c);
    CaseStatement* getDefaultCase();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//SWITCH_CASE_H
