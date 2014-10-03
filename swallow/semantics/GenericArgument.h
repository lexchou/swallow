#ifndef GENERIC_ARGUMENT_H
#define GENERIC_ARGUMENT_H

#include "swift_conf.h"
#include <map>
#include <vector>
SWIFT_NS_BEGIN

typedef std::shared_ptr<class Type> TypePtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
class GenericArgument
{
public:
    GenericArgument(const GenericDefinitionPtr& definition);
public:

    void add(const TypePtr& type);

    std::vector<TypePtr>::iterator begin() { return types.begin();}
    std::vector<TypePtr>::iterator end() { return types.end();}

    size_t size() const { return types.size();}

    TypePtr get(size_t index) const;
    TypePtr get(const std::wstring& name) const;
private:
    GenericDefinitionPtr definition;
    std::vector<TypePtr> types;
    std::vector<std::wstring, TypePtr> typeByName;

};




SWIFT_NS_END


#endif//GENERIC_ARGUMENT_H