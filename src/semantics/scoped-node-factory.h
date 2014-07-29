#ifndef SCOPED_NODE_FACTORY_H
#define SCOPED_NODE_FACTORY_H
#include "ast/node-factory.h"

SWIFT_NS_BEGIN

class ScopedNodeFactory : public NodeFactory
{
public:
    virtual IdentifierPtr createIdentifier(const SourceInfo& state);
    virtual EnumDefPtr createEnum(const SourceInfo& state);
    virtual StructDefPtr createStruct(const SourceInfo& state);
    virtual ClassDefPtr createClass(const SourceInfo& state);
    virtual ProtocolDefPtr createProtocol(const SourceInfo& state);
    virtual ExtensionDefPtr createExtension(const SourceInfo& state);
    virtual ProgramPtr createProgram();
    virtual CodeBlockPtr createCodeBlock(const SourceInfo& state);
    virtual ClosurePtr createClosure(const SourceInfo& state);

    virtual ConstantPtr createConstant(const SourceInfo& state);
    virtual VariablePtr createVariable(const SourceInfo& state);
    virtual FunctionDefPtr createFunction(const SourceInfo& state);

};

SWIFT_NS_END

#endif//SCOPED_NODE_FACTORY_H