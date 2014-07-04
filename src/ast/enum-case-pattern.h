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
public:
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
    
    void setAssociatedBinding(Tuple* tuple);
    Tuple* getAssociatedBinding();
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//ENUM_CASE_PATTERN_H
