#ifndef VALUE_BINDING_H
#define VALUE_BINDING_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ValueBinding : public Statement
{
public:
    ValueBinding();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//VALUE_BINDING_H
