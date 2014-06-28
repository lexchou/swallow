#ifndef DEINITIALIZER_H
#define DEINITIALIZER_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class DeinitializerDef : public Declaration
{
public:
    DeinitializerDef();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//DEINITIALIZER_H
