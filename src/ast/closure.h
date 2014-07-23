#ifndef CLOSURE_H
#define CLOSURE_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class Parameters;
class TypeNode;
class Statement;
class Closure : public Expression
{
public:
    enum CaptureSpecifier
    {
        _,
        Weak,
        Unowned,
        Unowned_Safe,
        Unowned_Unsafe
    };
public:
    Closure();
    ~Closure();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
public:
    CaptureSpecifier getCaptureSpecifier()const;
    void setCaptureSpecifier(CaptureSpecifier val);
    
    void setCapture(Expression* capture);
    Expression* getCapture();
    
    void setParameters(Parameters* val);
    Parameters* getParameters();
    
    void setReturnType(TypeNode* val);
    TypeNode* getReturnType();
    
    void addStatement(Statement* st);
    int numStatement()const;
    Statement* getStatement(int i);

    std::vector<Statement*>::iterator begin(){return statements.begin();}
    std::vector<Statement*>::iterator end(){return statements.end();}
public:
    CaptureSpecifier captureSpecifier;
    Expression* capture;
    Parameters* parameters;
    TypeNode* returnType;
    std::vector<Statement*> statements;
    
};

SWIFT_NS_END

#endif//CLOSURE_H
