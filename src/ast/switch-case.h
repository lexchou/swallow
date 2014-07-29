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
    void setControlExpression(const ExpressionPtr& expr);
    ExpressionPtr getControlExpression();
    
    void addCase(const CaseStatementPtr& c);
    int numCases();
    CaseStatementPtr getCase(int idx);
    
    void setDefaultCase(const CaseStatementPtr& c);
    CaseStatementPtr getDefaultCase();
public:
    std::vector<CaseStatementPtr>::iterator begin() {return cases.begin();}
    std::vector<CaseStatementPtr>::iterator end() {return cases.end();}
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr controlExpression;
    std::vector<CaseStatementPtr> cases;
    CaseStatementPtr defaultCase;
};

SWIFT_NS_END

#endif//SWITCH_CASE_H
