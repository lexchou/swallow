#ifndef SWITCH_CASE_H
#define SWITCH_CASE_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class CaseStatement;
class SwitchCase : public Statement
{
public:
    SwitchCase();
    ~SwitchCase();
public:
    void setControlExpression(Expression* expr);
    Expression* getControlExpression();
    
    void addCase(CaseStatement* c);
    int numCases();
    CaseStatement* getCase(int idx);
    
    void setDefaultCase(CaseStatement* c);
    CaseStatement* getDefaultCase();
public:
    virtual void serialize(std::wostream& out);
private:
    Expression* controlExpression;
    std::vector<CaseStatement*> cases;
    CaseStatement* defaultCase;
};

SWIFT_NS_END

#endif//SWITCH_CASE_H
