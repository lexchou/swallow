#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

class Variables;
class Constant;
class Assignment;
class ClassDef;
class StructDef;
class EnumDef;
class ProtocolDef;
class ExtensionDef;
class FunctionDef;

class NodeVisitor
{
public:
    virtual ~NodeVisitor(){}
public:
    virtual void visitVariables(Variables* node){}
    virtual void visitConstants(Constant* node){}
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