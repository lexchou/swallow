#ifndef OPERATOR_DEF_H
#define OPERATOR_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class OperatorDef : public Declaration
{
public:
    OperatorDef();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//OPERATOR_DEF_H
