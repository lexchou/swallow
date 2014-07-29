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
        PatternPtr condition;
        ExpressionPtr guard;
        Condition(PatternPtr condition, ExpressionPtr guard)
        :condition(condition), guard(guard)
        {}
    };
public:
    CaseStatement();
    ~CaseStatement();
public:
    void addCondition(const PatternPtr& condition, const ExpressionPtr& guard);
    int numConditions()const;
    const Condition& getCondition(int i);
    const std::vector<Condition>& getConditions(){return conditions;}

    std::vector<StatementPtr>::iterator begin(){return statements.begin();}
    std::vector<StatementPtr>::iterator end(){return statements.end();}
    
    void addStatement(const StatementPtr& statement);
    StatementPtr getStatement(int idx);
    int numStatements();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::vector<Condition> conditions;
    std::vector<StatementPtr> statements;
};

SWIFT_NS_END

#endif//CASE_STATEMENT_H
