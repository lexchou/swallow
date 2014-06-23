#ifndef SUBSCRIPT_H
#define SUBSCRIPT_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class Subscript : public ExpressionNode
{
public:
    Subscript(ExpressionNode* self, ExpressionNode* index);
public:
    void setSelf(ExpressionNode* self);
    ExpressionNode* getSelf();
    
    void setIndex(ExpressionNode* index);
    ExpressionNode* getIndex();

    void setIndex(int idx, ExpressionNode* index);
    ExpressionNode* getIndex(int idx);
    
    void addIndex(ExpressionNode* index);
    int numIndices();
public:
    virtual void serialize(std::wostream& out);
    
};

SWIFT_NS_END



#endif//SUBSCRIPT_H