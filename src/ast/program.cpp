
#include "program.h"
#include "statement.h"
#include "node-visitor.h"
USE_SWIFT_NS


Program::Program()
    :Node(NodeType::Program)
{
}

Program::~Program()
{
    SafeDeleteAll(statements);
}

void Program::serialize(std::wostream& out)
{

}


void Program::accept(NodeVisitor* visitor)
{
    visitor->visitProgram(this);
}
void Program::addStatement(Statement* statement)
{
    statements.push_back(statement);
}
int Program::numStatements()const
{
    return statements.size();
}
Statement* Program::getStatement(int n)
{
    return statements[n];
}

