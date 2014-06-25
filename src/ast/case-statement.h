#ifndef CASE_STATEMENT_H
#define CASE_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ExpressionNode;
class CaseStatement : public Statement
{
public:
    CaseStatement();
public:
    void setCondition(ExpressionNode* exr);
    ExpressionNode* getCondition();
    
    void setGuard(ExpressionNode* expr);
    ExpressionNode* getGuard();
    
    void addStatement(Statement* statement);
    Statement* getStatement(int idx);
    int numStatements();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//CASE_STATEMENT_H
