#ifndef VARIABLE_H
#define VARIABLE_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Variable : public Declaration
{
public:
    Variable();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//VARIABLE_H
