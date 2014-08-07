#include "type.h"

USE_SWIFT_NS


Type::Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType)
:name(name), category(category), keyType(keyType), valueType(valueType)
{

}



TypePtr Type::newDictionaryType(TypePtr keyType, TypePtr valueType)
{
    return TypePtr(new Type(L"", Dictionary, keyType, valueType));
}
TypePtr Type::newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference)
{
    TypePtr ret = TypePtr(new Type(name, category, nullptr, nullptr));
    ret->reference = reference;
    return ret;
}

TypePtr Type::newTypeReference(const TypePtr& innerType)
{
    TypePtr ret(new Type(L"", Reference, nullptr, nullptr));
    ret->innerType = innerType;
    return ret;
}


TypePtr Type::newArrayType(TypePtr elementType)
{
    return TypePtr(new Type(L"", Array, nullptr, elementType));
}

TypePtr Type::newTuple(const std::vector<TypePtr>& types)
{
    Type* ret = new Type(L"", Tuple, nullptr, nullptr);
    ret->elementTypes = types;
    return TypePtr(ret);
}

TypePtr Type::newFunction(const std::vector<TypePtr>& parameters, const TypePtr& returnType)
{
    Type* ret = new Type(L"", Function, nullptr, nullptr);
    ret->parameterTypes = parameters;
    ret->returnType = returnType;
    return TypePtr(ret);
}

Type::Category Type::getCategory()const
{
    return category;
}
bool Type::isPrimitive()const
{
    return category == Primitive;
}
bool Type::isArray()const
{
    return category == Array;
}
bool Type::isDictionary() const
{
    return category == Dictionary;
}
bool Type::isTuple()const
{
    return category == Tuple;
}


bool Type::isValueType()const
{
    switch(category)
    {
        case Primitive:
        case Tuple:
        case Struct:
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
TypePtr Type::getParentType()
{
    return parentType;
}

const std::wstring& Type::getModuleName() const
{
    return moduleName;
}
TypePtr Type::getElementType()
{
    return getElementType(0);
}
TypePtr Type::getElementType(int index)
{
    if(index >= elementTypes.size() || index < 0)
        return nullptr;
    return elementTypes[index];
}
int Type::numElementTypes()const
{
    return elementTypes.size();
}
TypePtr Type::getKeyType()
{
    return keyType;
}

TypePtr Type::getValueType()
{
    return valueType;
}
TypeDeclarationPtr Type::getReference()
{
    return reference;
}

TypePtr Type::getInnerType()
{
    return innerType;
}


TypePtr Type::getReturnType()
{
    return returnType;
}


const std::vector<TypePtr>& Type::getParameterTypes()
{
    return parameterTypes;
}

bool Type::operator ==(const Type& rhs)const
{
    if(category != rhs.category)
        return false;
    if(moduleName != rhs.moduleName)
        return false;

    switch(category)
    {
        case Primitive:
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
            if(reference != rhs.reference)
                return false;
            //TODO: check generic parameters
            break;
        case Array:
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
        case Dictionary:
            if(*keyType != *rhs.keyType)
                return false;
            if(*valueType != *rhs.valueType)
                return false;
            break;
        case Reference:
            if(*innerType != *rhs.innerType)
                return false;
            break;
        case Function:
        case Closure:
        {
            if(*returnType != *rhs.returnType)
                return false;
            if(parameterTypes.size() != rhs.parameterTypes.size())
                return false;
            auto iter = parameterTypes.begin(), iter2 = rhs.parameterTypes.begin();
            for(; iter != parameterTypes.end(); iter++, iter2++)
            {
                if(*iter != *iter2)
                    return false;
            }
            break;
        }
    }

    return true;
}

bool Type::operator !=(const Type& rhs)const
{
    return !this->operator==(rhs);
}