#include "LabeledStatement.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


LabeledStatement::LabeledStatement()
    :Statement(NodeType::LabeledStatement), statement(NULL)
{
}
LabeledStatement::~LabeledStatement()
{

}

void LabeledStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitLabeledStatement);
}


void LabeledStatement::setLabel(const std::wstring& label)
{
    this->label = label;
}
const std::wstring& LabeledStatement::getLabel() const
{
    return label;
}

void LabeledStatement::setStatement(const StatementPtr& statement)
{
    this->statement = statement;
}

StatementPtr LabeledStatement::getStatement()
{
    return statement;
}
