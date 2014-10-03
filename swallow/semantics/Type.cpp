#include "Type.h"
#include <cassert>
#include "GenericDefinition.h"
#include "GenericArgument.h"

USE_SWIFT_NS

using namespace std;

Type::Type(Category category)
:category(category)
{
    _containsAssociatedType = -1;
    _containsSelfType = -1;
    variadicParameters = false;
    inheritantDepth = 0;
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
    static TypePtr placeholder(new Type(Type::GenericParameter));
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
        ret->addParentTypesFrom(parentType);
    for(const TypePtr& t : protocols)
    {
        ret->addParentTypesFrom(t);
    }
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

TypePtr Type::newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters, const GenericDefinitionPtr& generic)
{
    Type* ret = new Type(Function);
    ret->parameters = parameters;
    ret->returnType = returnType;
    ret->variadicParameters = hasVariadicParameters;
    ret->genericDefinition = generic;
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const GenericArgumentPtr& arguments)
{
    Type* ret = new Type(Specialized);
    ret->innerType = innerType;
    ret->genericArguments = arguments;
    //copy members from innerType and update types with given argument
    //replace parents and protocols to apply with the generic arguments
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const TypePtr& argument)
{
    GenericArgumentPtr arguments(new GenericArgument());
    arguments->add(argument);

    Type* ret = new Type(Specialized);
    ret->innerType = innerType;
    ret->genericArguments = arguments;
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
void Type::setParentType(const TypePtr &type)
{
    this->parentType = type;
}

const std::vector<TypePtr>& Type::getProtocols() const
{
    return protocols;
}
void Type::addProtocol(const TypePtr &protocol)
{
    protocols.push_back(protocol);
    auto iter = parents.find(protocol);
    if(iter == parents.end())
        parents.insert(make_pair(protocol, 1));
    else
        iter->second = 1;
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

const GenericArgumentPtr& Type::getGenericArguments() const
{
    return genericArguments;
}
const GenericDefinitionPtr& Type::getGenericDefinition() const
{
    return genericDefinition;
}

FunctionOverloadedSymbolPtr Type::getInitializer()
{
    return initializer;
}

bool Type::isKindOf(const TypePtr &protocolOrBase) const
{
    assert(protocolOrBase != nullptr);
    if(protocolOrBase->getCategory() != Class && protocolOrBase->getCategory() != Protocol)
        return false;
    auto iter = parents.find(protocolOrBase);
    return iter != parents.end();
}

void Type::addParentTypesFrom(const TypePtr& type)
{
    for(auto parent : type->parents)
    {
        addParentType(parent.first, parent.second + 1);
    }
    addParentType(type, 1);
}
void Type::addParentType(const TypePtr& type, int distance)
{
    auto iter = parents.find(type);
    if(iter == parents.end())
    {
        parents.insert(std::make_pair(type, distance));
    }
    else if(iter->second > distance)
    {
        iter->second = distance;
    }
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

TypePtr Type::getType()
{
    return nullptr;
}
void Type::addMember(const SymbolPtr& symbol)
{
    addMember(symbol->getName(), symbol);
}

void Type::addMember(const std::wstring& name, const SymbolPtr& member)
{
    symbols.insert(std::make_pair(name, member));
    if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(member))
    {
        storedProperties.push_back(s);
    }
}

SymbolPtr Type::getMember(const std::wstring& name) const
{
    SymbolPtr ret = getDeclaredMember(name);
    //look for directly declared member
    if(ret)
        return ret;
    //check in base type
    if(category != Protocol && category != GenericParameter)
    {
        if(parentType != nullptr)
        {
            ret = parentType->getMember(name);
            if(ret)
                return ret;
        }
        //TODO: check from extensions
        return nullptr;
    }
    else
    {
        //look from all protocols
        for(auto entry : parents)
        {
            ret = entry.first->getDeclaredMember(name);
            if(ret)
                return ret;
        }
        return nullptr;
    }
}
SymbolPtr Type::getDeclaredMember(const std::wstring& name) const
{
    auto iter = symbols.find(name);
    if(iter == symbols.end())
        return nullptr;
    return iter->second;
}
const Type::SymbolMap& Type::getDeclaredMembers() const
{
    return symbols;
}

bool Type::containsSelfType() const
{
    if (_containsSelfType != -1)
        return _containsSelfType == 1;
    bool ret = containsSelfTypeImpl();
    _containsSelfType = ret ? 1 : 0;
    return ret;
}
bool Type::containsSelfTypeImpl() const
{
    if(category == Self)
        return true;
    if(category == Function || category == Closure)
    {
        if(returnType && returnType->containsSelfType())
            return true;
        //check parameter type
        for(auto param : parameters)
        {
            if(param.type->containsSelfType())
                return true;
        }
    }
    else if(category == Specialized)
    {
        if(innerType->containsSelfType())
            return true;
        if(genericDefinition)
        {
            for(const TypePtr& param : genericDefinition->getParameters())
            {
                if(param->containsSelfType())
                    return true;
            }
        }

    }
    else if(category == Tuple)
    {
        for(const TypePtr& t : elementTypes)
        {
            if(t->containsSelfType())
                return true;
        }
    }
    else
    {
        //check all symbols
        for(auto member : symbols)
        {
            if(TypePtr type = dynamic_pointer_cast<Type>(member.second))
            {
                if(type->containsSelfType())
                    return true;
            }
            else if(TypePtr type = member.second->getType())
            {
                if(type->containsSelfType())
                    return true;
            }
        }
    }
    return false;
}

bool Type::containsAssociatedType() const
{
    if(_containsAssociatedType != -1)
        return _containsAssociatedType == 1;
    _containsAssociatedType = 0;
    for(auto entry : symbols)
    {
        if(dynamic_pointer_cast<Type>(entry.second))
        {
            _containsAssociatedType = 1;
            return true;
        }
    }
    return false;
}
/**
* Check if this type's generic arguments contains generic parameters
*/
bool Type::containsGenericParameters() const
{
    if(genericArguments == nullptr)
        return false;
    for(const TypePtr& type : *this->genericArguments)
    {
        if(type->category == GenericParameter)
            return true;
        if(type->containsGenericParameters())
            return true;
    }
    return false;
}
TypePtr Type::getAssociatedType(const std::wstring& name) const
{
    SymbolPtr symbol = getMember(name);
    if(symbol == nullptr)
        return nullptr;
    TypePtr ret = dynamic_pointer_cast<Type>(symbol);
    return ret;
}
TypePtr Type::getDeclaredAssociatedType(const std::wstring& name) const
{
    SymbolPtr symbol = getDeclaredMember(name);
    if(symbol == nullptr)
        return nullptr;
    TypePtr ret = dynamic_pointer_cast<Type>(symbol);
    return ret;
}
const std::vector<SymbolPtr>& Type::getDeclaredStoredProperties() const
{
    return storedProperties;
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
            if(!isGenericDefinitionEquals(genericDefinition, rhs.genericDefinition))
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
            if(genericArguments != nullptr && rhs.genericArguments != nullptr)
            {
                if (genericArguments->size() != rhs.genericArguments->size())
                    return false;
                auto iter = genericArguments->begin();
                auto iter2 = rhs.genericArguments->begin();
                for (; iter != genericArguments->end(); iter++, iter2++)
                {
                    if (!Type::equals(*iter, *iter2))
                        return false;
                }
            }
            if(genericArguments == nullptr && rhs.genericArguments == nullptr)
                return true;
            return false;
        }
        case GenericParameter:
            return true;
    }

    return true;
}

bool Type::operator !=(const Type& rhs)const
{
    return !this->operator==(rhs);
}
