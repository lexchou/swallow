#ifndef LITERAL_NODES_H
#define LITERAL_NODES_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral : public ExpressionNode
{
public:
    BooleanLiteral(bool val);
public:
    virtual void serialize(std::wostream& out);
private:
    bool value;
};
class IntegerLiteral : public ExpressionNode
{
public:
    IntegerLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};
class FloatLiteral : public ExpressionNode
{
public:
    FloatLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};
class StringLiteral : public ExpressionNode
{
public:
    StringLiteral(const std::wstring& val);
public:
    const std::wstring& toString() const { return value;}
    virtual void serialize(std::wostream& out);
private:
    std::wstring value;
};
class CompileConstant : public ExpressionNode
{
public:
    CompileConstant(const std::wstring& name, const std::wstring& value);
public:
    const std::wstring& getName() const;
    const std::wstring& getValue() const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    std::wstring value;
};

SWIFT_NS_END



#endif//LITERAL_NODES_H
