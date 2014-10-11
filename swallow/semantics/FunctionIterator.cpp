#include "FunctionIterator.h"
#include "Type.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"

USE_SWIFT_NS

using namespace std;

FunctionIterator::iterator::iterator(const TypePtr& type, const std::wstring& name)
:type(type), name(name)
{
    if(type)
    {
        moveNext();
    }
}
bool FunctionIterator::iterator::operator!=(const FunctionIterator::iterator& rhs)const
{
    return value != rhs.value;
}
const FunctionIterator::iterator& FunctionIterator::iterator::operator++()
{
    if(!type)
        return *this;
    value = *begin;
    begin++;
    if(begin == end)
        moveNext();
    return *this;
}
void FunctionIterator::iterator::moveNext()
{
    //move to the parent type
    for(;type; type = type->getParentType())
    {
        FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(type->getDeclaredMember(name));
        if(!funcs)
            continue;//this type doesn't defined the function, going upward
        begin = funcs->begin();
        end = funcs->end();
        value = *begin;
        break;
    }
}
FunctionSymbolPtr FunctionIterator::iterator::operator*()
{
    return value;
}

FunctionIterator::FunctionIterator(const TypePtr& type, const std::wstring& name)
:type(type), name(name)
{

}
FunctionIterator::iterator FunctionIterator::begin()
{
    return iterator(type, name);
}
FunctionIterator::iterator FunctionIterator::end()
{
    return iterator(nullptr, L"");
}