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
    void setArgumentsType(const TupleTypePtr& argumentsType);
    void setReturnType(const TypeNodePtr& retType);


    TupleTypePtr getArgumentsType();
    TypeNodePtr getReturnType();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    TupleTypePtr argumentsType;
    TypeNodePtr returnType;
};

SWIFT_NS_END

#endif//FUNCTION_TYPE_H
