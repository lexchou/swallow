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
public:
    void setConvenience(bool convenience);
    bool isConvenience()const;
    
    
    void setParameters(Parameters* parameters);
    Parameters* getParameters();
    
    void setBody(CodeBlock* body);
    CodeBlock* getBody();
public:
    virtual void serialize(std::wostream& out);
private:
    bool convenience;
};

SWIFT_NS_END

#endif//INITIALIZER_DEF_H
