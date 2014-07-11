
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