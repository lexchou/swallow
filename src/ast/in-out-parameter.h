#ifndef IN_OUT_PARAMETER_H
#define IN_OUT_PARAMETER_H
#include "unary-operator.h"
SWIFT_NS_BEGIN

class Identifier;
class InOutParameter : public UnaryOperator
{
public:
    InOutParameter(Identifier* identifier);
};

SWIFT_NS_END

#endif//IN_OUT_PARAMETER_H
