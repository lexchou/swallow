#ifndef SYMBOL_H
#define SYMBOL_H
#include "swift_conf.h"
#include <string>

SWIFT_NS_BEGIN

class Symbol
{
public:
    Symbol(){}
    virtual ~Symbol(){}
public:
    virtual const std::wstring& getName() const = 0;

};

SWIFT_NS_END


#endif//SYMBOL_H