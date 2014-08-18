#ifndef LABELED_STATEMENT_H
#define LABELED_STATEMENT_H
#include "Statement.h"
#include <string>

SWIFT_NS_BEGIN

class LabeledStatement : public Statement
{
public:
    LabeledStatement();
    ~LabeledStatement();
public:
    void setLabel(const std::wstring& label);
    const std::wstring& getLabel() const;
    
    void setStatement(const StatementPtr& statement);
    StatementPtr getStatement();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring label;
    StatementPtr statement;
};

SWIFT_NS_END

#endif//LABELED_STATEMENT_H
