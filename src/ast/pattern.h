#ifndef PATTERN_H
#define PATTERN_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern : public Statement
{
public:
    Pattern(int numChildren);
public:
};

SWIFT_NS_END

#endif//PATTERN_H
