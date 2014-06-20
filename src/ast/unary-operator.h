#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H
#include "node.h"
#include <string>
SWIFT_NS_BEGIN

class UnaryOperator : public Node
{
public:
    UnaryOperator(const std::wstring& op);
public:
    void setOperand(Node* node){set(0, node);}
    Node* getOperand(){return get(0);}
private:
    std::wstring op;
};

SWIFT_NS_END

#endif//UNARY_OPERATOR_H
