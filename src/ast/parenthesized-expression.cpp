#include "parenthesized-expression.h"
#include "expression.h"
USE_SWIFT_NS


ParenthesizedExpression::ParenthesizedExpression()
    :Expression(0)
{
    
}
void ParenthesizedExpression::append(const std::wstring& name, Expression* expr)
{
    names.push_back(name);
    children.push_back(expr);
}
void ParenthesizedExpression::append(Expression* expr)
{
    append(L"", expr);
}
void ParenthesizedExpression::serialize(std::wostream& out)
{
    out<<L"(";
    std::vector<Node*>::iterator iter = children.begin();
    std::vector<std::wstring>::iterator name = names.begin();
    for(; iter != children.end(); iter++, name++)
    {
        if(iter != children.begin())
            out<<L", ";
        if(!name->empty())
            out<<*name<<" : ";
        (*iter)->serialize(out);
    }
    
    out<<L")";
}
std::wstring ParenthesizedExpression::getName(int idx)
{
    if(idx <0 || idx >= names.size())
        return L"";
    return names[idx];
}