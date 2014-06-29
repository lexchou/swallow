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
public:
    void setParameters(Parameters* params);
    Parameters* getParameters();
    
    void setReturnType(TypeNode* type);
    TypeNode* getReturnType();
    
    void setReturnTypeAttributes(const Attributes& attrs);
    const Attributes& getReturnTypeAttributes()const;
    
    void setGetter(CodeBlock* getter);
    CodeBlock* getGetter();
    
    void setSetter(CodeBlock* setter);
    CodeBlock* getSetter();
    
    void setSetterName(const std::wstring& name);
    const std::wstring& getSetterName()const;
        
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring setterName;
    Attributes returnTypeAttributes;
};

SWIFT_NS_END

#endif//SUBSCRIPT_DEF_H
