#ifndef DECLARATION_H
#define DECLARATION_H
#include "statement.h"
#include <string>
#include <vector>

SWIFT_NS_BEGIN

class Attribute;
class Declaration : public Statement
{
public:
    Declaration();
    ~Declaration();
public:
    void setAttributes(const std::vector<Attribute*>& attrs);
    const std::vector<Attribute*>& getAttributes();
protected:
    int getSpecifiers();
    void setSpecifiers(int specifiers);
    
public:
    virtual void serialize(std::wostream& out);
    
protected:
    std::vector<Attribute*> attributes;
    int specifiers;
};

SWIFT_NS_END

#endif//DECLARATION_H
