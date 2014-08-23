#ifndef PARAMETER_H
#define PARAMETER_H
#include "Node.h"
#include <string>
#include "Attribute.h"
#include "ast-decl.h"

SWIFT_NS_BEGIN

class TypeNode;
class Expression;
class Type;
typedef std::shared_ptr<Type> TypePtr;
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
    virtual void accept(NodeVisitor* visitor);
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
    
    void setDeclaredType(const TypeNodePtr& type);
    TypeNodePtr getDeclaredType();


    void setType(const TypePtr& type);
    TypePtr getType();

    void setDefaultValue(const ExpressionPtr& def);
    ExpressionPtr getDefaultValue();
    
private:
    bool inout;
    bool shorthandExternalName;
    Accessibility accessibility;
    std::wstring externalName;
    std::wstring localName;
    Attributes typeAttributes;
    TypeNodePtr declaredType;
    TypePtr type;
    ExpressionPtr defaultValue;
};

SWIFT_NS_END

#endif//PARAMETER_H
