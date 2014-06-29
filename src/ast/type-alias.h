#ifndef TYPE_ALIAS_H
#define TYPE_ALIAS_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode;
class TypeAlias : public Declaration
{
public:
    TypeAlias();
public:
    const std::wstring& getName()const;
    void setName(const std::wstring& name);
    
    void setType(TypeNode* type);
    TypeNode* getType();
    
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring name;
};

SWIFT_NS_END

#endif//TYPE_ALIAS_H
