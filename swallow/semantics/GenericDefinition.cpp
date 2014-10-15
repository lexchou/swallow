#include "GenericDefinition.h"
#include <cassert>
#include "Type.h"
#include "SymbolScope.h"

USE_SWIFT_NS
using namespace std;


int GenericDefinition::validate(const std::wstring& typeName, const TypePtr& typeToTest, TypePtr& expectedType) const
{
    auto iter = constraints.find(typeName);
    if(iter == constraints.end())
        return 0;
    NodeDefPtr node = iter->second;
    return validate(node, typeToTest, expectedType);
}
int GenericDefinition::validate(const NodeDefPtr& node, const TypePtr& typeToTest, TypePtr& expectedType) const
{
    for(const Constraint& constraint : node->constraints)
    {
        //check current type
        switch(constraint.type)
        {
            case EqualsTo:
                if(!Type::equals(typeToTest, constraint.reference))
                {
                    expectedType = constraint.reference;
                    return 0;//not equal to specified file
                }
                break;
            case DerivedFrom:
                if(!typeToTest->isKindOf(constraint.reference))
                {
                    expectedType = constraint.reference;
                    return 0;
                }
                break;
        }
        //check children type
        for(auto entry : node->children)
        {
            TypePtr childType = typeToTest->getAssociatedType(entry.first);
            assert(childType != nullptr);
            int n = validate(entry.second, childType, expectedType);
            if(n <= 0)
                return n;
        }
    }
    return 1;
}
void GenericDefinition::add(const std::wstring& alias, const TypePtr& type)
{
    assert(type != nullptr);
    TypePtr old = get(alias);
    assert(old == nullptr);
    int index = (int)typeParameters.size();
    typeParameters.push_back(Parameter(index, alias, type));
    NodeDefPtr node(new NodeDef());
    node->type = type;
    node->index = index;
    constraints.insert(make_pair(alias, node));
}
void GenericDefinition::addConstraint(const std::list<std::wstring>& type, ConstraintType constraint, const TypePtr& referenceType)
{
    assert(!type.empty());
    auto begin = type.begin();
    NodeDefPtr node = constraints[*begin];
    assert(node != nullptr);
    for(begin++; begin != type.end(); begin++)
    {
        NodeDefPtr next = node->children[*begin];
        if(!next)
        {
            TypePtr childType = node->type->getAssociatedType(*begin);
            assert(childType != nullptr);
            next = NodeDefPtr(new NodeDef());
            next->type = childType;
            node->children.insert(make_pair(*begin, next));
        }
    }
    node->constraints.push_back(Constraint(constraint, referenceType));
}
TypePtr GenericDefinition::get(const std::wstring &alias)
{
    auto iter = constraints.find(alias);
    if(iter == constraints.end())
        return nullptr;
    return iter->second->type;
}
void GenericDefinition::registerTo(SymbolScope* scope)
{
    for(auto entry : constraints)
    {
        scope->addSymbol(entry.first, entry.second->type);
    }

}
size_t GenericDefinition::numParameters() const
{
    return typeParameters.size();
}
const std::vector<GenericDefinition::Parameter> GenericDefinition::getParameters()const
{
    return typeParameters;
}


bool GenericDefinition::equals(const GenericDefinitionPtr& rhs) const
{
    if(rhs == nullptr)
        return false;
    //check type
    if(typeParameters.size() != rhs->typeParameters.size())
        return false;
    auto iter1 = typeParameters.begin();
    auto iter2 = rhs->typeParameters.begin();
    for(; iter1 != typeParameters.end(); iter1++, iter2++)
    {
        if(!Type::equals(iter1->type, iter2->type))
            return false;
    }
    //check constraint

    return true;
}
