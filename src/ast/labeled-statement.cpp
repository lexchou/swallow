
#include "labeled-statement.h"
USE_SWIFT_NS


LabeledStatement::LabeledStatement()
    :Statement(1)
{
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
    set(0, statement);
}

Statement* LabeledStatement::getStatement()
{
    return static_cast<Statement*>(get(0));
}