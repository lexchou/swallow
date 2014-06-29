#ifndef VARIABLE_H
#define VARIABLE_H
#include "declaration.h"
#include "attribute.h"
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
    
    
    void setName(Pattern* pattern);
    Pattern* getName();
    
    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;
    
    void setType(TypeNode* t);
    TypeNode* getType();
    
    
    void setInitializer(Expression* expr);
    Expression* getInitializer();
    
    void setSetter(CodeBlock* setter);
    CodeBlock* getSetter();
    
    void setSetterName(const std::wstring& name);
    const std::wstring& getSetterName();
    
    void setGetter(CodeBlock* getter);
    CodeBlock* getGetter();
    
    void setWillSet(CodeBlock* willSet);
    CodeBlock* getWillSet();
    
    void setDidSet(CodeBlock* didSet);
    CodeBlock* getDidSet();

public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
    std::wstring setterName;
    Attributes typeAttributes;
};

SWIFT_NS_END

#endif//VARIABLE_H
