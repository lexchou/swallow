#ifndef EXTENSION_DEF_H
#define EXTENSION_DEF_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;
class ExtensionDef : public Declaration
{
public:
    ExtensionDef();
    ~ExtensionDef();
public:
    virtual void serialize(std::wostream& out);
public:
    void addAdoptedProtocol(TypeIdentifier* protocol);
    int numAdoptedProtocols();
    TypeIdentifier* getAdoptedProtocol(int i);
    
    void setIdentifier(TypeIdentifier* id);
    TypeIdentifier* getIdentifier();
    
    void addDeclaration(Declaration* decl);
    int numDeclarations();
    Declaration* getDeclaration(int i);
private:
    std::vector<TypeIdentifier*> adoptedProtocols;
};

SWIFT_NS_END

#endif//EXTENSION_DEF_H
