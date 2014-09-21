#include "Type.h"
#include <cassert>
#include "GenericDefinition.h"

USE_SWIFT_NS


Type::Type(Category category)
:category(category)
{

}

bool Type::equals(const TypePtr& lhs, const TypePtr& rhs)
{
    if(lhs == rhs)
        return true;
    if(lhs == nullptr || rhs == nullptr)
        return false;
    return *lhs == *rhs;
}
/**
* A type place holder for protocol's typealias
*/
const TypePtr& Type::getPlaceHolder()
{
    static TypePtr placeholder(new Type(Type::Placeholder));
    return placeholder;
}

TypePtr Type::newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference, const TypePtr& parentType, const std::vector<TypePtr>& protocols, const GenericDefinitionPtr& generic)
{
    TypePtr ret = TypePtr(new Type(category));
    ret->name = name;
    ret->reference = reference;
    ret->parentType = parentType;
    ret->protocols = protocols;
    ret->genericDefinition = generic;
    if(parentType)
        ret->inheritantDepth = parentType->inheritantDepth + 1;
    return ret;
}

TypePtr Type::newTypeReference(const TypePtr& innerType)
{
    TypePtr ret(new Type(MetaType));
    ret->innerType = innerType;
    return ret;
}


TypePtr Type::newTuple(const std::vector<TypePtr>& types)
{
    Type* ret = new Type(Tuple);
    ret->elementTypes = types;
    return TypePtr(ret);
}

TypePtr Type::newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters)
{
    Type* ret = new Type(Function);
    ret->parameters = parameters;
    ret->returnType = returnType;
    ret->variadicParameters = hasVariadicParameters;
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const std::vector<TypePtr>& arguments)
{
    Type* ret = new Type(Specialized);
    ret->innerType = innerType;
    ret->genericArguments = arguments;
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const TypePtr& argument)
{
    Type* ret = new Type(Specialized);
    ret->innerType = innerType;
    ret->genericArguments.push_back(argument);
    return TypePtr(ret);
}


/**
 * Gets the common parent class between current class and rhs with the minimum inheritance distance.
 */
TypePtr Type::getCommonParent(const TypePtr& rhs)
{
    if(rhs == nullptr || this == rhs.get())
        return rhs;
    TypePtr a;
    if(parentType)
        a = parentType->getCommonParent(rhs);

    TypePtr pthis = std::static_pointer_cast<Type>(shared_from_this());
    TypePtr b = rhs->getCommonParent(parentType);
    if(a && b)
    {
        if(a->inheritantDepth > b->inheritantDepth)
            return a;
        else
            return b;
    }
    if(a)
        return a;
    if(b)
        return b;
    return nullptr;
}



Type::Category Type::getCategory()const
{
    return category;
}


/**
 * return true if the type is a class/struct/protocol/enum
 */
bool Type::isObjectType()const
{
    return category == Class || category == Struct || category == Enum || category == Protocol;
}

bool Type::isValueType()const
{
    switch(category)
    {
        case Tuple:
        case Struct:
            //TODO: String is not value type
            return true;
        case Aggregate:
            if(name == L"String")
                return false;
            return true;
        default:
            return false;
    }
}


const std::wstring& Type::getName()const
{
    return name;
}

const std::wstring& Type::getFullName() const
{
    return fullName;
}
TypePtr Type::getParentType()const
{
    return parentType;
}

const std::vector<TypePtr>& Type::getProtocols() const
{
    return protocols;
}

const std::wstring& Type::getModuleName() const
{
    return moduleName;
}
TypePtr Type::getElementType() const
{
    return getElementType(0);
}
TypePtr Type::getElementType(int index) const
{
    if(index >= elementTypes.size() || index < 0)
        return nullptr;
    return elementTypes[index];
}
int Type::numElementTypes()const
{
    return elementTypes.size();
}

TypeDeclarationPtr Type::getReference()const
{
    if(reference.expired())
        return nullptr;
    return reference.lock();
}

TypePtr Type::getInnerType() const
{
    return innerType;
}


TypePtr Type::getReturnType() const
{
    return returnType;
}


const std::vector<Type::Parameter>& Type::getParameters() const
{
    return parameters;
}

bool Type::hasVariadicParameters()const
{
    return variadicParameters;
}

bool Type::isGenericType()const
{
    return this->category != Specialized && genericDefinition != nullptr;
}

std::vector<TypePtr>& Type::getGenericArguments()
{
    return genericArguments;
}
const GenericDefinitionPtr& Type::getGenericDefinition()
{
    return genericDefinition;
}

FunctionOverloadedSymbolPtr Type::getInitializer()
{
    return initializer;
}
Type::SymbolMap& Type::getSymbols()
{
    return symbols;
}

static bool isGenericDefinitionEquals(const GenericDefinitionPtr& a, const GenericDefinitionPtr& b)
{
    if(a == b)
        return true;
    if(a == nullptr || b == nullptr)
        return false;
    if(a->equals(b))
        return true;
    return false;
}

bool Type::operator ==(const Type& rhs)const
{
    if(category != rhs.category)
        return false;
    if(moduleName != rhs.moduleName)
        return false;

    switch(category)
    {
        case Aggregate:
        case Class:
        case Struct:
        case Protocol:
        case Extension:
        case Enum:
            //check name
            if (fullName != rhs.fullName)
                return false;
            if(name != rhs.name)
                return false;
            if(getReference() != rhs.getReference())
                return false;
            if(!isGenericDefinitionEquals(genericDefinition, rhs.genericDefinition))
                return false;
            break;
        case Tuple:
        {
            if (!elementTypes.empty() && !rhs.elementTypes.empty())
                return false;
            auto iter = elementTypes.begin(), iter2 = rhs.elementTypes.begin();
            for (; iter != elementTypes.end(); iter++, iter2++)
            {
                if (*iter != *iter2)
                    return false;
            }
            break;
        }
        case MetaType:
            if(*innerType != *rhs.innerType)
                return false;
            break;
        case Function:
        case Closure:
        {
            if(!Type::equals(returnType, rhs.returnType))
                return false;
            if(parameters.size() != rhs.parameters.size())
                return false;
            if(variadicParameters != rhs.variadicParameters)
                return false;
            auto iter = parameters.begin(), iter2 = rhs.parameters.begin();
            for(; iter != parameters.end(); iter++, iter2++)
            {
                if(iter->name != iter2->name)
                    return false;
                if(iter->inout != iter2->inout)
                    return false;
                if(*iter->type != *iter2->type)
                    return false;
            }
            break;
        }
        case Specialized:
        {
            if (!Type::equals(innerType, rhs.innerType))
                return false;
            if (genericArguments.size() != rhs.genericArguments.size())
                return false;
            auto iter = genericArguments.begin();
            auto iter2 = rhs.genericArguments.begin();
            for(; iter != genericArguments.end(); iter++, iter2++)
            {
                if(!Type::equals(*iter, *iter2))
                    return false;
            }
            return true;
        }
        case Placeholder:
            return true;
    }

    return true;
}

bool Type::operator !=(const Type& rhs)const
{
    return !this->operator==(rhs);
}
