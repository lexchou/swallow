#ifndef PATTERN_H
#define PATTERN_H
#include "statement.h"
#include "semantics/type.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern : public Statement
{
public:
    Pattern(NodeType::T nodeType);
public:
    void setType(TypePtr type);
    TypePtr getType();

private:
    TypePtr type;
};
typedef std::shared_ptr<Pattern> PatternPtr;

SWIFT_NS_END

#endif//PATTERN_H
