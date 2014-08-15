#ifndef SUBSCRIPT_DEF_H
#define SUBSCRIPT_DEF_H
#include "declaration.h"
#include <string>
#include "attribute.h"

SWIFT_NS_BEGIN
class CodeBlock;
class Parameters;
class TypeNode;
class SubscriptDef : public Declaration
{
public:
    SubscriptDef();
    ~SubscriptDef();
public:
    void setParameters(const ParametersPtr& params);
    ParametersPtr getParameters();
    
    void setReturnType(const TypeNodePtr& type);
    TypeNodePtr getReturnType();
    
    void setReturnTypeAttributes(const Attributes& attrs);
    const Attributes& getReturnTypeAttributes()const;
    
    void setGetter(const CodeBlockPtr& getter);
    CodeBlockPtr getGetter();
    
    void setSetter(const CodeBlockPtr& setter);
    CodeBlockPtr getSetter();
    
    void setSetterName(const std::wstring& name);
    const std::wstring& getSetterName()const;
        
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring setterName;
    Attributes returnTypeAttributes;
    ParametersPtr parameters;
    TypeNodePtr returnType;
    CodeBlockPtr getter;
    CodeBlockPtr setter;
};

SWIFT_NS_END

#endif//SUBSCRIPT_DEF_H
