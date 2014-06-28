
#include "tuple-type.h"
#include "attribute.h"
USE_SWIFT_NS


TupleType::TupleType()
    :TypeNode(0)
{
}
static void serializeAttributes(TypeNode* type, std::wostream& out)
{
    const std::vector<Attribute*>& attrs = type->getAttributes();
    if(attrs.empty())
        return;
    std::vector<Attribute*>::const_iterator iter = attrs.begin();
    for(; iter != attrs.end(); iter++)
    {
        (*iter)->serialize(out);
        out<<L" ";
    }
}
void TupleType::serialize(std::wostream& out)
{
    out<<L"(";
    Children::iterator itype = children.begin();
    std::vector<TupleElementMeta>::iterator imeta = metas.begin();
    for(; itype != children.end(); itype++, imeta++)
    {
        TypeNode* type = static_cast<TypeNode*>(*itype);
        if(itype != children.begin())
        {
            out<<L", ";
        }
        if(imeta->name.empty())
        {
            serializeAttributes(type, out);
            if(imeta->inout)
                out<<L"inout ";
            (*itype)->serialize(out);
        }
        else
        {
            if(imeta->inout)
                out<<L"inout ";
            out<<imeta->name<<L" : ";
            serializeAttributes(type, out);
            (*itype)->serialize(out);
        }
    }
    
    out<<L")";
}

void TupleType::add(bool inout, const std::wstring& name, TypeNode* type)
{
    children.push_back(type);
    metas.push_back(TupleElementMeta(inout, name));
}
int TupleType::numElements()
{
    return children.size();
}
TypeNode* TupleType::getElementType(int i)
{
    return static_cast<TypeNode*>(get(i));
}


void TupleType::setVariadicParameters(bool val)
{
    variadicParameters = val;
}
bool TupleType::getVariadicParameters()const
{
    return variadicParameters;
}
