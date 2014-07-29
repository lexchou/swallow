#ifndef INITIALIZER_DEF_H
#define INITIALIZER_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Parameters;
class CodeBlock;
class InitializerDef : public Declaration
{
public:
    InitializerDef();
    ~InitializerDef();
public:
    using Declaration::setGenericParameters;
    using Declaration::getGenericParameters;
    void setConvenience(bool convenience);
    bool isConvenience()const;
    
    
    void setParameters(const ParametersPtr& parameters);
    ParametersPtr getParameters();
    
    void setBody(const CodeBlockPtr& body);
    CodeBlockPtr getBody();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    bool convenience;
    ParametersPtr parameters;
    CodeBlockPtr body;
};

SWIFT_NS_END

#endif//INITIALIZER_DEF_H
