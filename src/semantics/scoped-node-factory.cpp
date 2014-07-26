#include "scoped-node-factory.h"
#include "scoped-nodes.h"
#include "symbol-identifier.h"
USE_SWIFT_NS


Identifier* ScopedNodeFactory::createIdentifier(const SourceInfo& state, const std::wstring& op)
{
    SymbolIdentifier* ret = _(state, new SymbolIdentifier(op));
    return ret;
}
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
Constant* ScopedNodeFactory::createConstant(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers)
{
    Constant* ret = _(state, new SymboledConstant());
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}
Variable* ScopedNodeFactory::createVariable(const SourceInfo& state)
{
    return _(state, new SymboledVariable());
}
FunctionDef* ScopedNodeFactory::createFunction(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers)
{
    FunctionDef* ret = _(state, new SymboledFunction());
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}
