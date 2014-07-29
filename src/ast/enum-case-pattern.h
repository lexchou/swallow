#ifndef ENUM_CASE_PATTERN_H
#define ENUM_CASE_PATTERN_H
#include "pattern.h"
#include <string>

SWIFT_NS_BEGIN

class Tuple;
class EnumCasePattern : public Pattern
{
public:
    EnumCasePattern();
    ~EnumCasePattern();
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
    
    void setAssociatedBinding(const TuplePtr& tuple);
    TuplePtr getAssociatedBinding();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring name;
    TuplePtr associatedBinding;
};

SWIFT_NS_END

#endif//ENUM_CASE_PATTERN_H
