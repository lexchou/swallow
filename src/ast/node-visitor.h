#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

class NodeVisitor
{
public:
    virtual ~NodeVisitor(){}
public:
    virtual void visitAssignment(Assignment* node){}
    virtual void visitClass(ClassDef* node){}
    virtual void visitStruct(StructDef* node){}
    virtual void visitEnum(EnumDef* node){}
    virtual void visitProtocol(ProtocolDef* node){}
    virtual void visitExtension(ExtensionDef* node){}
    virtual void visitFunction(FunctionDef* node){}

};


SWIFT_NS_END
#endif//NODE_VISITOR_H