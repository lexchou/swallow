#ifndef LITERAL_NODES_H
#define LITERAL_NODES_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral : public Node
{
public:
    BooleanLiteral(bool val);
private:
    bool value;
};
class IntegerLiteral : public Node
{
public:
    IntegerLiteral(const std::wstring& val);
private:
    std::wstring value;
};
class FloatLiteral : public Node
{
public:
    FloatLiteral(const std::wstring& val);
private:
    std::wstring value;
};
class StringLiteral : public Node
{
public:
    StringLiteral(const std::wstring& val);
private:
    std::wstring value;

};

SWIFT_NS_END



#endif//LITERAL_NODES_H
