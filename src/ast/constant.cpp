
#include "constant.h"
#include "pattern.h"
#include "expression.h"
USE_SWIFT_NS


Constant::Constant()
    :Declaration(0)
{
}
void Constant::serialize(std::wostream& out)
{
}

void Constant::add(Pattern* pattern, Expression* initializer)
{
    children.push_back(pattern);
    children.push_back(initializer);
}
int Constant::numPairs()
{
    return numChildren() >> 1;
}
std::pair<Pattern*, Expression*> Constant::getPair(int i)
{
    Pattern* p = static_cast<Pattern*>(get(i << 1));
    Expression* expr = static_cast<Expression*>(get((i << 1) + 1));
    return std::make_pair(p, expr);
}
