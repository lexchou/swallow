
#include "constant.h"
#include "pattern.h"
#include "expression.h"
USE_SWIFT_NS


Constant::Constant()
    :Declaration(NodeType::Constant)
{
}
Constant::~Constant()
{
    SafeDeleteAll(pairs);
}
void Constant::serialize(std::wostream& out)
{
}

void Constant::add(Pattern* pattern, Expression* initializer)
{
    pairs.push_back(pattern);
    pairs.push_back(initializer);
}
int Constant::numPairs()
{
    return pairs.size() >> 1;
}
std::pair<Pattern*, Expression*> Constant::getPair(int i)
{
    Pattern* p = static_cast<Pattern*>(pairs[i << 1]);
    Expression* expr = static_cast<Expression*>(pairs[(i << 1) + 1]);
    return std::make_pair(p, expr);
}
