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
    ~TypeAlias();
public:
    const std::wstring& getName()const;
    void setName(const std::wstring& name);
    
    void setType(TypeNodePtr type);
    TypeNodePtr getType();
    
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring name;
    TypeNodePtr type;
};

SWIFT_NS_END

#endif//TYPE_ALIAS_H
