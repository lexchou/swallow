#include "Declaration.h"
#include "Attribute.h"
#include <algorithm>
USE_SWIFT_NS


Declaration::Declaration(NodeType::T nodeType)
    :Statement(nodeType), specifiers(0), genericParameters(NULL)
{
}
Declaration::~Declaration()
{
}


void Declaration::setAttributes(const std::vector<AttributePtr>& attrs)
{
    this->attributes.clear();
    std::copy(attrs.begin(), attrs.end(), std::back_inserter(attributes));
}

const std::vector<AttributePtr>& Declaration::getAttributes()
{
    return attributes;
}
AttributePtr Declaration::getAttribute(const wchar_t* name)
{
    for(const AttributePtr& attr : attributes)
    {
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


GenericParametersPtr Declaration::getGenericParameters()
{
    return genericParameters;
}
void Declaration::setGenericParameters(const GenericParametersPtr& val)
{
    genericParameters = val;
}
