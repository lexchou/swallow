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
    std::vector<CaseStatement*>::iterator begin() {return cases.begin();}
    std::vector<CaseStatement*>::iterator end() {return cases.end();}
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    Expression* controlExpression;
    std::vector<CaseStatement*> cases;
    CaseStatement* defaultCase;
};

SWIFT_NS_END

#endif//SWITCH_CASE_H
