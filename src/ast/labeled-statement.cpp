
#include "labeled-statement.h"
USE_SWIFT_NS


LabeledStatement::LabeledStatement()
    :statement(NULL)
{
}
LabeledStatement::~LabeledStatement()
{
    SafeDelete(statement);
}
void LabeledStatement::serialize(std::wostream& out)
{
    out<<label<<L":";
    getStatement()->serialize(out);
}



void LabeledStatement::setLabel(const std::wstring& label)
{
    this->label = label;
}
const std::wstring& LabeledStatement::getLabel() const
{
    return label;
}

void LabeledStatement::setStatement(Statement* statement)
{
    this->statement = statement;
}

Statement* LabeledStatement::getStatement()
{
    return statement;
}