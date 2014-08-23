#ifndef IN_OUT_PARAMETER_H
#define IN_OUT_PARAMETER_H
#include "UnaryOperator.h"
#include "ast-decl.h"
SWIFT_NS_BEGIN

class InOutParameter : public UnaryOperator
{
public:
    InOutParameter();
};

SWIFT_NS_END

#endif//IN_OUT_PARAMETER_H
