#ifndef CASE_STATEMENT_H
#define CASE_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class Pattern;
class CaseStatement : public Statement
{
public:
    CaseStatement();
public:
    void setCondition(Pattern* exr);
    Pattern* getCondition();
    
    void setGuard(Expression* expr);
    Expression* getGuard();
    
    void addStatement(Statement* statement);
    Statement* getStatement(int idx);
    int numStatements();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//CASE_STATEMENT_H
