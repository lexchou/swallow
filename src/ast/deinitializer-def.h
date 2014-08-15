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
    ~DeinitializerDef();
public:
    CodeBlockPtr getBody();
    void setBody(const CodeBlockPtr& body);
public:
    virtual void accept(NodeVisitor* visitor);
private:
    CodeBlockPtr body;
};

SWIFT_NS_END

#endif//DEINITIALIZER_H
