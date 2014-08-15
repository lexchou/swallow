#ifndef CONSTANT_H
#define CONSTANT_H
#include "value-binding.h"
#include <string>

SWIFT_NS_BEGIN

class Constant : public ValueBinding
{
public:
    Constant();
    ~Constant();
public:
    virtual void accept(NodeVisitor* visitor);
};

SWIFT_NS_END

#endif//CONSTANT_H
