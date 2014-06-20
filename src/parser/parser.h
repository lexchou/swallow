#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

class Tokenizer;
class Node;
class SymbolRegistry;
class NodeFactory;
class Parser
{
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    SymbolRegistry* symbolRegistry;
};



SWIFT_NS_END

#endif//PARSER_H
