#ifndef SCOPED_NODE_FACTORY_H
#define SCOPED_NODE_FACTORY_H
#include "ast/NodeFactory.h"

SWIFT_NS_BEGIN

class ScopedNodeFactory : public NodeFactory
{
public:
    virtual IdentifierPtr createIdentifier(const SourceInfo& state) override;
    virtual EnumDefPtr createEnum(const SourceInfo& state) override;
    virtual StructDefPtr createStruct(const SourceInfo& state) override;
    virtual ClassDefPtr createClass(const SourceInfo& state) override;
    virtual ProtocolDefPtr createProtocol(const SourceInfo& state) override;
    virtual ExtensionDefPtr createExtension(const SourceInfo& state) override;
    virtual ProgramPtr createProgram() override;
    virtual CodeBlockPtr createCodeBlock(const SourceInfo& state) override;
    virtual ClosurePtr createClosure(const SourceInfo& state) override;

    virtual ConstantPtr createConstant(const SourceInfo& state) override;
    virtual VariablePtr createVariable(const SourceInfo& state) override;
    virtual FunctionDefPtr createFunction(const SourceInfo& state) override;


};

SWIFT_NS_END

#endif//SCOPED_NODE_FACTORY_H
