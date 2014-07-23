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
    void addStatement(Statement* st);
    int numStatements();
    Statement* getStatement(int idx);
    
    void setAttributes(const Attributes& attrs);
    const Attributes& getAttributes()const;
public:
    std::vector<Statement*>::iterator begin() { return statements.begin();}
    std::vector<Statement*>::iterator end() { return statements.end();}
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    Attributes attributes;
    std::vector<Statement*> statements;
};

SWIFT_NS_END

#endif//CODE_BLOCK_H
