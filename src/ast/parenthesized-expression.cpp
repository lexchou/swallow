#include "parenthesized-expression.h"
#include "expression.h"
#include "node-visitor.h"
USE_SWIFT_NS


ParenthesizedExpression::ParenthesizedExpression()
    :Expression(NodeType::ParenthesizedExpression)
{
    
}

ParenthesizedExpression::~ParenthesizedExpression()
{
}

void ParenthesizedExpression::append(const std::wstring& name, const ExpressionPtr& expr)
{
    expressions.push_back(std::make_pair(name, expr));
}
void ParenthesizedExpression::append(const ExpressionPtr& expr)
{
    append(L"", expr);
}
int ParenthesizedExpression::numExpressions()const
{
    return expressions.size();
}
void ParenthesizedExpression::serialize(std::wostream& out)
{
    out<<L"(";
    std::vector<std::pair<std::wstring, ExpressionPtr> >::iterator iter = expressions.begin();
    for(; iter != expressions.end(); iter++)
    {
        if(iter != expressions.begin())
            out<<L", ";
        if(!iter->first.empty())
            out<<iter->first<<" : ";
        (iter->second)->serialize(out);
    }
    
    out<<L")";
}

void ParenthesizedExpression::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitParenthesizedExpression);
}

std::wstring ParenthesizedExpression::getName(int idx)
{
    if(idx <0 || idx >= expressions.size())
        return L"";
    return expressions[idx].first;
}
ExpressionPtr ParenthesizedExpression::get(int idx)
{
    if(idx <0 || idx >= expressions.size())
        return NULL;
    return expressions[idx].second;
}