
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
}

void Program::serialize(std::wostream& out)
{

}


void Program::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitProgram);
}
void Program::addStatement(const StatementPtr& statement)
{
    statements.push_back(statement);
}
int Program::numStatements()const
{
    return statements.size();
}
StatementPtr Program::getStatement(int n)
{
    return statements[n];
}

