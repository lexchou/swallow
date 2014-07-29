
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

static void serializeAttributes(const TypeNodePtr& type, std::wostream& out)
{
    const std::vector<AttributePtr>& attrs = type->getAttributes();
    if(attrs.empty())
        return;
    std::vector<AttributePtr>::const_iterator iter = attrs.begin();
    for(; iter != attrs.end(); iter++)
    {
        (*iter)->serialize(out);
        out<<L" ";
    }
}
void TupleType::serialize(std::wostream& out)
{
    out<<L"(";
    std::vector<TupleElement>::iterator iter = elements.begin();
    for(; iter != elements.end(); iter++)
    {
        TypeNodePtr type = iter->type;
        if(iter != elements.begin())
        {
            out<<L", ";
        }
        if(iter->name.empty())
        {
            serializeAttributes(type, out);
            if(iter->inout)
                out<<L"inout ";
            iter->type->serialize(out);
        }
        else
        {
            if(iter->inout)
                out<<L"inout ";
            out<<iter->name<<L" : ";
            serializeAttributes(type, out);
            iter->type->serialize(out);
        }
    }
    
    out<<L")";
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
