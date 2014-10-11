#ifndef FUNCTION_ITERATOR_H
#define FUNCTION_ITERATOR_H
#include "swift_conf.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN

typedef std::shared_ptr<class Type> TypePtr;
typedef std::shared_ptr<class FunctionSymbol> FunctionSymbolPtr;

/**
 * This iterator will iterate the function by name through out the inheritance hierarchy
 */
class FunctionIterator
{
public:
    class iterator
    {
    private:
        TypePtr type;
        std::wstring name;
        std::vector<FunctionSymbolPtr>::iterator begin;
        std::vector<FunctionSymbolPtr>::iterator end;
        FunctionSymbolPtr value;
    public:
        iterator(const TypePtr& type, const std::wstring& name);
    public:
        bool operator!=(const iterator& rhs)const;
        const iterator& operator++();
        FunctionSymbolPtr operator*();
        void moveNext();
    };
public:
    FunctionIterator(const TypePtr& type, const std::wstring& name);
public:
    iterator begin();
    iterator end();
private:
    TypePtr type;
    std::wstring name;
};

SWIFT_NS_END


#endif//FUNCTION_ITERATOR_H