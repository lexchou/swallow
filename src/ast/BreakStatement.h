#ifndef BREAK_STATEMENT_H
#define BREAK_STATEMENT_H
#include "Statement.h"
#include <string>

SWIFT_NS_BEGIN

class BreakStatement : public Statement
{
public:
    BreakStatement();
public:
    void setLoop(const std::wstring& loop);
    const std::wstring& getLoop()const;
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring loop;
};

SWIFT_NS_END

#endif//BREAK_STATEMENT_H
