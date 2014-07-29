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
    
    void setCapture(const ExpressionPtr& capture);
    ExpressionPtr getCapture();
    
    void setParameters(const ParametersPtr& val);
    ParametersPtr getParameters();
    
    void setReturnType(const TypeNodePtr& val);
    TypeNodePtr getReturnType();
    
    void addStatement(const StatementPtr& st);
    int numStatement()const;
    StatementPtr getStatement(int i);

    std::vector<StatementPtr>::iterator begin(){return statements.begin();}
    std::vector<StatementPtr>::iterator end(){return statements.end();}
public:
    CaptureSpecifier captureSpecifier;
    ExpressionPtr capture;
    ParametersPtr parameters;
    TypeNodePtr returnType;
    std::vector<StatementPtr> statements;
    
};

SWIFT_NS_END

#endif//CLOSURE_H
