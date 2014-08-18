#include "ScopedNodeFactory.h"
#include "ScopedNodes.h"
USE_SWIFT_NS


IdentifierPtr ScopedNodeFactory::createIdentifier(const SourceInfo& state)
{
    return NodeFactory::createIdentifier(state);
}
EnumDefPtr ScopedNodeFactory::createEnum(const SourceInfo& state)
{
    return _(state, new ScopedEnum());
}
StructDefPtr ScopedNodeFactory::createStruct(const SourceInfo& state)
{
    return _(state, new ScopedStruct());
}
ClassDefPtr ScopedNodeFactory::createClass(const SourceInfo& state)
{
    return _(state, new ScopedClass());
}
ProtocolDefPtr ScopedNodeFactory::createProtocol(const SourceInfo& state)
{
    return _(state, new ScopedProtocol());
}
ExtensionDefPtr ScopedNodeFactory::createExtension(const SourceInfo& state)
{
    return _(state, new ScopedExtension());
}
ProgramPtr ScopedNodeFactory::createProgram()
{
    return _(SourceInfo(), new ScopedProgram());
}
CodeBlockPtr ScopedNodeFactory::createCodeBlock(const SourceInfo& state)
{
    return _(state, new ScopedCodeBlock());
}
ClosurePtr ScopedNodeFactory::createClosure(const SourceInfo& state)
{
    return _(state, new ScopedClosure());
}
ConstantPtr ScopedNodeFactory::createConstant(const SourceInfo& state)
{
    return _(state, new SymboledConstant());
}
VariablePtr ScopedNodeFactory::createVariable(const SourceInfo& state)
{
    return _(state, new SymboledVariable());
}
FunctionDefPtr ScopedNodeFactory::createFunction(const SourceInfo& state)
{
    return _(state, new SymboledFunction());
}
