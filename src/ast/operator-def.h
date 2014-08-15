#ifndef OPERATOR_DEF_H
#define OPERATOR_DEF_H
#include "declaration.h"
#include "swift_types.h"
#include <string>

SWIFT_NS_BEGIN

class OperatorDef : public Declaration
{
public:
    OperatorDef();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
    
    void setType(OperatorType::T type);
    OperatorType::T getType();
    
    void setPrecedence(int val);
    int getPrecedence() const;
    
    void setAssociativity(Associativity::T val);
    Associativity::T getAssociativity();
    
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring name;
    OperatorType::T type;
    int precedence;
    Associativity::T associativity;
};

SWIFT_NS_END

#endif//OPERATOR_DEF_H
