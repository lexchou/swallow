
#include "declaration.h"
#include "attribute.h"
#include "generic-parameters.h"
#include <algorithm>
USE_SWIFT_NS


Declaration::Declaration()
    :specifiers(0), genericParameters(NULL)
{
}
Declaration::~Declaration()
{
    SafeDeleteAll(attributes);
    SafeDelete(genericParameters);
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
Attribute* Declaration::getAttribute(const wchar_t* name)
{
    std::vector<Attribute*>::iterator iter = attributes.begin();
    for(; iter != attributes.end(); iter++)
    {
        Attribute* attr = *iter;
        if(attr->getName() == name)
            return attr;
    }
    return NULL;
}


int Declaration::getSpecifiers()
{
    return specifiers;
}
void Declaration::setSpecifiers(int specifiers)
{
    this->specifiers = specifiers;
}


GenericParameters* Declaration::getGenericParameters()
{
    return genericParameters;
}
void Declaration::setGenericParameters(GenericParameters* val)
{
    genericParameters = val;
}