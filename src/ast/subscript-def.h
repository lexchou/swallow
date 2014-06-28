#ifndef SUBSCRIPT_DEF_H
#define SUBSCRIPT_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class SubscriptDef : public Declaration
{
public:
    SubscriptDef();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//SUBSCRIPT_DEF_H
