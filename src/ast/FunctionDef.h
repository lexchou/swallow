#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H
#include "Declaration.h"
#include "Attribute.h"
#include <string>

SWIFT_NS_BEGIN

class Parameters;
class CodeBlock;
class TypeNode;
typedef std::shared_ptr<class Type> TypePtr;
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
    
    using Declaration::setGenericParameters;
    using Declaration::getGenericParameters;
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
    
    
    
    void addParameters(const ParametersPtr&);
    int numParameters();
    ParametersPtr getParameters(int i);
    const std::vector<ParametersPtr>&  getParametersList()const{return parametersList;}

    
    void setReturnType(const TypeNodePtr&);
    TypeNodePtr getReturnType();
    
    void setReturnTypeAttributes(const Attributes& attrs);
    const Attributes& getReturnTypeAttributes()const;
    
    void setBody(const CodeBlockPtr& body);
    CodeBlockPtr getBody();

    const TypePtr& getType()const;
    void setType(const TypePtr& type);

public:
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring name;
    Attributes returnAttributes;
    std::vector<ParametersPtr> parametersList;
    TypeNodePtr returnType;
    CodeBlockPtr body;
    TypePtr type;
};

SWIFT_NS_END

#endif//FUNCTION_DEF_H
