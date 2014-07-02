
#include "declaration.h"
#include "attribute.h"
#include <algorithm>
USE_SWIFT_NS


Declaration::Declaration(int numChildren)
    :Statement(numChildren)
{
}
Declaration::~Declaration()
{
    std::vector<Attribute*>::iterator iter = attributes.begin();
    for(; iter != attributes.end(); iter++)
    {
        delete *iter;
    }
}
void Declaration::serialize(std::wostream& out)
{
}


void Declaration::setAttributes(const std::vector<Attribute*>& attrs)
{
    this->attributes.clear();
    std::copy(attrs.begin(), attrs.end(), std::back_inserter(attributes));
}

const std::vector<Attribute*>& Declaration::getAttributes()
{
    return attributes;
}


int Declaration::getSpecifiers()
{
    return specifiers;
}
void Declaration::setSpecifiers(int specifiers)
{
    this->specifiers = specifiers;
}