#ifndef VARIABLE_H
#define VARIABLE_H
#include "declaration.h"
#include "attribute.h"
#include "semantics/semantic-types.h"
#include <string>

SWIFT_NS_BEGIN
class CodeBlock;
class Expression;
class Pattern;
class TypeNode;
class Variable : public Declaration
{
public:
    Variable();
    ~Variable();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
    
    
    void setName(const PatternPtr& pattern);
    PatternPtr getName();
    
    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;
    
    void setDeclaredType(const TypeNodePtr& t);
    TypeNodePtr getDeclaredType();
    
    
    void setInitializer(const ExpressionPtr& expr);
    ExpressionPtr getInitializer();
    
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
    TypePtr getType();
    void setType(const TypePtr& type);
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring setterName;
    std::wstring willSetSetter;
    std::wstring didSetSetter;
    Attributes typeAttributes;
    
    PatternPtr name;
    TypeNodePtr declaredType;
    ExpressionPtr initializer;
    CodeBlockPtr getter;
    CodeBlockPtr setter;
    CodeBlockPtr willSet;
    CodeBlockPtr didSet;

    TypePtr type;
};

SWIFT_NS_END

#endif//VARIABLE_H
