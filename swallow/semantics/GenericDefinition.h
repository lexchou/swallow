#ifndef GENERIC_DEFINITION_H
#define GENERIC_DEFINITION_H

#include "swift_conf.h"
#include <vector>
#include <map>
#include <string>

SWIFT_NS_BEGIN

typedef std::shared_ptr<class Type> TypePtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
class GenericDefinition
{
public:
    void add(const std::wstring& alias, const TypePtr& type);
    TypePtr get(const std::wstring& alias);
    size_t numParameters() const;
    bool equals(const GenericDefinitionPtr& rhs) const;
    const std::map<std::wstring, TypePtr>& getTypeMap() const;
private:
    std::vector<TypePtr> typeParameters;
    std::map<std::wstring, TypePtr> typeByAlias;
};

SWIFT_NS_END


#endif//GENERIC_DEFINITION_H