#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H
#include "Statement.h"
#include "Attribute.h"
#include <string>

SWIFT_NS_BEGIN
typedef std::shared_ptr<class Type> TypePtr;

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

    const TypePtr& getType()const;
    void setType(const TypePtr& type);
public:
    std::vector<StatementPtr>::iterator begin() { return statements.begin();}
    std::vector<StatementPtr>::iterator end() { return statements.end();}
public:
    virtual void accept(NodeVisitor* visitor);
private:
    Attributes attributes;
    std::vector<StatementPtr> statements;
    TypePtr type;
};

SWIFT_NS_END

#endif//CODE_BLOCK_H
