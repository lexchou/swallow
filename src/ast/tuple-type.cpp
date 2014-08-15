
#include "tuple-type.h"
#include "attribute.h"
USE_SWIFT_NS


TupleType::TupleType()
    :TypeNode(NodeType::TupleType)
{
    variadicParameters = false;
}
TupleType::~TupleType()
{
}


void TupleType::add(bool inout, const std::wstring& name, const TypeNodePtr& type)
{
    elements.push_back(TupleElement(inout, name, type));
}
int TupleType::numElements()
{
    return elements.size();
}
TypeNodePtr TupleType::getElementType(int i)
{
    return elements[i].type;
}
const TupleType::TupleElement& TupleType::getElement(int i)
{
    return elements[i];
}

void TupleType::setVariadicParameters(bool val)
{
    variadicParameters = val;
}
bool TupleType::getVariadicParameters()const
{
    return variadicParameters;
}
