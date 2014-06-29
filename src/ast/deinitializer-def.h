#ifndef DEINITIALIZER_H
#define DEINITIALIZER_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class CodeBlock;
class DeinitializerDef : public Declaration
{
public:
    DeinitializerDef();
public:
    CodeBlock* getBody();
    void setBody(CodeBlock* body);
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//DEINITIALIZER_H
