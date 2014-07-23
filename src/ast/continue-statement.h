#ifndef CONTINUE_STATEMENT_H
#define CONTINUE_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ContinueStatement : public Statement
{
public:
    ContinueStatement();
public:
    void setLoop(const std::wstring& loop);
    const std::wstring& getLoop()const;
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring loop;
};

SWIFT_NS_END

#endif//CONTINUE_STATEMENT_H
