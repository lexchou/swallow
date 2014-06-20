#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "node.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public Node
{
public:
    BinaryOperator(const std::wstring& op);
public:
    void setLHS(Node* node){set(0, node);}
    Node* getLHS(){return get(0);}

    void setRHS(Node* node){set(1, node);}
    Node* getRHS(){return get(1);}
private:
    std::wstring op;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
