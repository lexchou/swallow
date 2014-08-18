#ifndef VARIABLE_H
#define VARIABLE_H
#include "ValueBinding.h"
#include "Attribute.h"
#include <string>

SWIFT_NS_BEGIN
class CodeBlock;
class Expression;
class Pattern;
class TypeNode;
class Variable : public ValueBinding
{
public:
    Variable();
    ~Variable();
public:
    
    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;
    
    void setDeclaredType(const TypeNodePtr& t);
    TypeNodePtr getDeclaredType();
    
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
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring setterName;
    std::wstring willSetSetter;
    std::wstring didSetSetter;
    Attributes typeAttributes;
    
    TypeNodePtr declaredType;
    CodeBlockPtr getter;
    CodeBlockPtr setter;
    CodeBlockPtr willSet;
    CodeBlockPtr didSet;

};

SWIFT_NS_END

#endif//VARIABLE_H
