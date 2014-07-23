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
    struct Condition
    {
        Pattern* condition;
        Expression* guard;
        Condition(Pattern* condition, Expression* guard)
        :condition(condition), guard(guard)
        {}
    };
public:
    CaseStatement();
    ~CaseStatement();
public:
    void addCondition(Pattern* condition, Expression* guard);
    int numConditions()const;
    const Condition& getCondition(int i);
    const std::vector<Condition>& getConditions(){return conditions;}

    std::vector<Statement*>::iterator begin(){return statements.begin();}
    std::vector<Statement*>::iterator end(){return statements.end();}
    
    void addStatement(Statement* statement);
    Statement* getStatement(int idx);
    int numStatements();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::vector<Condition> conditions;
    std::vector<Statement*> statements;
};

SWIFT_NS_END

#endif//CASE_STATEMENT_H
