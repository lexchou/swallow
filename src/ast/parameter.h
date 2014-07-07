#ifndef PARAMETER_H
#define PARAMETER_H
#include "node.h"
#include <string>
#include "attribute.h"

SWIFT_NS_BEGIN

class TypeNode;
class Expression;
class Parameter : public Node
{
public:
    enum Accessibility
    {
        None,
        Constant,
        Variable
    };
public:
    Parameter();
    ~Parameter();
public:
    virtual void serialize(std::wostream& out);
public:
    void setInout(bool inout);
    bool isInout()const;
    
    void setAccessibility(Accessibility accessibility);
    Accessibility getAccessibility() const;
    
    void setExternalName(const std::wstring& name);
    const std::wstring& getExternalName()const;
    
    void setLocalName(const std::wstring& name);
    const std::wstring& getLocalName()const;
    
    void setShorthandExternalName(bool shorthandExternalName);
    bool isShorthandExternalName()const;
    
    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes()const;
    
    void setType(TypeNode* type);
    TypeNode* getType();
    
    void setDefaultValue(Expression* def);
    Expression* getDefaultValue();
    
private:
    bool inout;
    bool shorthandExternalName;
    Accessibility accessibility;
    std::wstring externalName;
    std::wstring localName;
    Attributes typeAttributes;
    TypeNode* type;
    Expression* defaultValue;
};

SWIFT_NS_END

#endif//PARAMETER_H
