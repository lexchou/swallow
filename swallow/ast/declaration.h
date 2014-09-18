#ifndef DECLARATION_H
#define DECLARATION_H
#include "Statement.h"
#include <string>
#include <vector>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Attribute;
class GenericParametersDef;
class Declaration : public Statement
{
public:
    Declaration(NodeType::T nodeType);
    ~Declaration();
public:
    void setAttributes(const std::vector<AttributePtr>& attrs);
    const std::vector<AttributePtr>& getAttributes();
    AttributePtr getAttribute(const wchar_t* name);
protected:
    int getSpecifiers();
    void setSpecifiers(int specifiers);
    
    GenericParametersDefPtr getGenericParametersDef();
    void setGenericParametersDef(const GenericParametersDefPtr& val);
    
protected:
    std::vector<AttributePtr> attributes;
    int specifiers;
    GenericParametersDefPtr genericParameters;
};

SWIFT_NS_END

#endif//DECLARATION_H
