#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class ReturnStatement : public Statement
{
public:
    ReturnStatement();
public:
    void setExpression(Expression* expr);
    Expression* getExpression();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//RETURN_STATEMENT_H
