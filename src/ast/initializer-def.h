#ifndef INITIALIZER_DEF_H
#define INITIALIZER_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class InitializerDef : public Declaration
{
public:
    InitializerDef();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//INITIALIZER_DEF_H
