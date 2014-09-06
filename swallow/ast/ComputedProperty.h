#ifndef COMPUTED_PROPERTY_H
#define COMPUTED_PROPERTY_H
#include "Declaration.h"
#include "Attribute.h"
#include <string>

SWIFT_NS_BEGIN
class CodeBlock;
class Expression;
class Pattern;
class TypeNode;
typedef std::shared_ptr<class Type> TypePtr;
class ComputedProperty : public Declaration
{
public:
    ComputedProperty();
    ~ComputedProperty();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:

    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;

    void setDeclaredType(const TypeNodePtr& t);
    TypeNodePtr getDeclaredType();

    const TypePtr& getType()const;
    void setType(const TypePtr& type);

    const std::wstring& getName()const;
    void setName(const std::wstring&);

    void setInitializer(const ExpressionPtr& initializer);
    const ExpressionPtr& getInitializer()const;
    void setSetter(const CodeBlockPtr& setter);
    CodeBlockPtr getSetter();
    
    void setSetterName(const std::wstring& name);
    const std::wstring& getSetterName();
    
    void setGetter(const CodeBlockPtr& getter);
    CodeBlockPtr getGetter();
    
    void setWillSet(const CodeBlockPtr& willSet);
    CodeBlockPtr getWillSet();
    void setWillSetSetter(const std::wstring& name);
    const std::wstring& getWillSetSetter()const;
    
    void setDidSet(const CodeBlockPtr& didSet);
    CodeBlockPtr getDidSet();
    
    void setDidSetSetter(const std::wstring& name);
    const std::wstring& getDidSetSetter()const;

public:
    virtual void accept(NodeVisitor* visitor) override;
private:
    std::wstring name;
    ExpressionPtr initializer;
    TypePtr type;
    TypeNodePtr declaredType;
    Attributes typeAttributes;
    std::wstring setterName;
    std::wstring willSetSetter;
    std::wstring didSetSetter;
    CodeBlockPtr getter;
    CodeBlockPtr setter;
    CodeBlockPtr willSet;
    CodeBlockPtr didSet;

};

SWIFT_NS_END

#endif//COMPUTED_PROPERTY_H
