#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN

class Attribute : public Node
{
public:
    Attribute();
public:
    void addBalancedToken(const std::wstring& token);
    void setName(const std::wstring& name);
    const std::wstring& getName()const;
private:
    std::vector<std::wstring> balancedTokens;
    std::wstring name;
public:
};


SWIFT_NS_END

#endif//ATTRIBUTE_H
