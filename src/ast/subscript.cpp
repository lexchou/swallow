#include "subscript.h"
USE_SWIFT_NS


Subscript::Subscript(ExpressionNode* self, ExpressionNode* index)
    :ExpressionNode(2)
{
    setSelf(self);
    setIndex(index);
}
void Subscript::setSelf(ExpressionNode* self)
{
    set(0, self);
}
ExpressionNode* Subscript::getSelf()
{
    return static_cast<ExpressionNode*>(get(0));
}

void Subscript::setIndex(ExpressionNode* index)
{
    set(1, index);
}
ExpressionNode* Subscript::getIndex()
{
    return static_cast<ExpressionNode*>(get(1));
}
void Subscript::setIndex(int idx, ExpressionNode* index)
{
    set(idx + 1, index);
}
ExpressionNode* Subscript::getIndex(int idx)
{
    return static_cast<ExpressionNode*>(get(idx + 1));
}

void Subscript::addIndex(ExpressionNode* index)
{
    children.push_back(index);
}
int Subscript::numIndices()
{
    return children.size() - 1;
}


void Subscript::serialize(std::wostream& out)
{
    getSelf()->serialize(out);
    out<<L"[";
    for(int i = 0; i < numIndices(); i++)
    {
        ExpressionNode* index = getIndex(i);
        if(i != 0)
            out<<L", ";
        index->serialize(out);
    }
    out<<L"]";
}

