#include <ast/node-visitor.h>
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
    for(std::pair<Pattern*, Expression*> pair : pairs)
    {
        SafeDelete(pair.first);
        SafeDelete(pair.second);
    }
    pairs.clear();

}
void Constant::serialize(std::wostream& out)
{
}

void Constant::add(Pattern* pattern, Expression* initializer)
{
    pairs.push_back(std::make_pair(pattern, initializer));
}
int Constant::numPairs()
{
    return pairs.size();
}
std::pair<Pattern*, Expression*> Constant::getPair(int i)
{
    return pairs[i];
}

void Constant::accept(NodeVisitor* visitor)
{
    visitor->visitConstants(this);
}