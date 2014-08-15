#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H
#include "statement.h"
#include <string>
#include "attribute.h"

SWIFT_NS_BEGIN

class CodeBlock : public Statement
{
public:
    CodeBlock();
    ~CodeBlock();
public:
    void addStatement(const StatementPtr& st);
    int numStatements();
    StatementPtr getStatement(int idx);
    
    void setAttributes(const Attributes& attrs);
    const Attributes& getAttributes()const;
public:
    std::vector<StatementPtr>::iterator begin() { return statements.begin();}
    std::vector<StatementPtr>::iterator end() { return statements.end();}
public:
    virtual void accept(NodeVisitor* visitor);
private:
    Attributes attributes;
    std::vector<StatementPtr> statements;
};

SWIFT_NS_END

#endif//CODE_BLOCK_H
