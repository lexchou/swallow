#ifndef TYPE_ALIAS_H
#define TYPE_ALIAS_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class TypeAlias : public Declaration
{
public:
    TypeAlias();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//TYPE_ALIAS_H
