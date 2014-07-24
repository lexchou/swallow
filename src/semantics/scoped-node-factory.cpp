#include "scoped-node-factory.h"
#include "scoped-nodes.h"
USE_SWIFT_NS


EnumDef* ScopedNodeFactory::createEnum(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ScopedEnum* ret = _(state, new ScopedEnum());
    ret->setAttributes(attrs);
    return ret;
}
StructDef* ScopedNodeFactory::createStruct(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ScopedStruct* ret = _(state, new ScopedStruct());
    ret->setAttributes(attrs);
    return ret;
}
ClassDef* ScopedNodeFactory::createClass(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ScopedClass* ret = _(state, new ScopedClass());
    ret->setAttributes(attrs);
    return ret;
}
ProtocolDef* ScopedNodeFactory::createProtocol(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ScopedProtocol* ret = _(state, new ScopedProtocol());
    ret->setAttributes(attrs);
    return ret;
}
ExtensionDef* ScopedNodeFactory::createExtension(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ScopedExtension* ret = _(state, new ScopedExtension());
    ret->setAttributes(attrs);
    return ret;
}
Program* ScopedNodeFactory::createProgram()
{
    return _(SourceInfo(), new ScopedProgram());
}
CodeBlock* ScopedNodeFactory::createCodeBlock(const SourceInfo& state)
{
    return _(state, new ScopedCodeBlock());
}
Closure* ScopedNodeFactory::createClosure(const SourceInfo& state)
{
    return _(state, new ScopedClosure());
}
