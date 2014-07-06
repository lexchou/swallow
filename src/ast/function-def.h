#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H
#include "declaration.h"
#include "attribute.h"
#include <string>

SWIFT_NS_BEGIN

class Parameters;
class CodeBlock;
class TypeNode;
class FunctionDef : public Declaration
{
public:
    FunctionDef();
    ~FunctionDef();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
    using Declaration::setAttributes;
    using Declaration::getAttributes;
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
    
    
    
    void addParameters(Parameters*);
    int numParameters();
    Parameters* getParameters(int i);
    
    void setReturnType(TypeNode*);
    TypeNode* getReturnType();
    
    void setReturnTypeAttributes(const Attributes& attrs);
    const Attributes& getReturnTypeAttributes()const;
    
    void setBody(CodeBlock* body);
    CodeBlock* getBody();
    
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    Attributes returnAttributes;
};

SWIFT_NS_END

#endif//FUNCTION_DEF_H
