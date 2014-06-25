#ifndef LABELED_STATEMENT_H
#define LABELED_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class LabeledStatement : public Statement
{
public:
    LabeledStatement();
public:
    void setLabel(const std::wstring& label);
    const std::wstring& getLabel() const;
    
    void setStatement(Statement* statement);
    Statement* getStatement();
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring label;
};

SWIFT_NS_END

#endif//LABELED_STATEMENT_H
