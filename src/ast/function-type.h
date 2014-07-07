#ifndef FUNCTION_TYPE_H
#define FUNCTION_TYPE_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class FunctionType : public TypeNode
{
public:
    FunctionType();
    ~FunctionType();
public:
    void setArgumentsType(TypeNode* argumentsType);
    void setReturnType(TypeNode* retType);
    
    
    TypeNode* getArgumentsType();
    TypeNode* getReturnType();
public:
    virtual void serialize(std::wostream& out);
private:
    TypeNode* argumentsType;
    TypeNode* returnType;
};

SWIFT_NS_END

#endif//FUNCTION_TYPE_H
