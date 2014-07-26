#ifndef SCOPED_NODE_FACTORY_H
#define SCOPED_NODE_FACTORY_H
#include "ast/node-factory.h"

SWIFT_NS_BEGIN

class ScopedNodeFactory : public NodeFactory
{
public:
    virtual Identifier* createIdentifier(const SourceInfo& state, const std::wstring& op);
    virtual EnumDef* createEnum(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual StructDef* createStruct(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ClassDef* createClass(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ProtocolDef* createProtocol(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual ExtensionDef* createExtension(const SourceInfo& state, const std::vector<Attribute*>& attrs);
    virtual Program* createProgram();
    virtual CodeBlock* createCodeBlock(const SourceInfo& state);
    virtual Closure* createClosure(const SourceInfo& state);

    virtual Constant* createConstant(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers);
    virtual Variable* createVariable(const SourceInfo& state);
    virtual FunctionDef* createFunction(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers);

};

SWIFT_NS_END

#endif//SCOPED_NODE_FACTORY_H