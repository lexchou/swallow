#ifndef PROGRAM_H
#define PROGRAM_H
#include "Node.h"
#include <string>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Statement;
class Program : public Node
{
public:
    Program();
    ~Program();
public:
    virtual void accept(NodeVisitor* visitor);


    void addStatement(const StatementPtr& statement);
    int numStatements()const;
    StatementPtr getStatement(int n);

    std::vector<StatementPtr>::iterator begin() { return statements.begin();}
    std::vector<StatementPtr>::iterator end() { return statements.end();}
private:
    std::vector<StatementPtr> statements;
};

SWIFT_NS_END

#endif//PROGRAM_H
