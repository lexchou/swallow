
#include "program.h"
#include "statement.h"
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
    for(Statement* st : statements)
    {
        st->accept(visitor);
    }
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

