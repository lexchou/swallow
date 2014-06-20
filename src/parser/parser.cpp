#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include <cstdlib>
#include <stack>
using namespace Swift;

Parser::Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry)
    :nodeFactory(nodeFactory), symbolRegistry(symbolRegistry)
{
    tokenizer = new Tokenizer(NULL);
}
Parser::~Parser()
{
    delete tokenizer;
}

Node* Parser::parse(const wchar_t* code)
{
    tokenizer->set(code);
    Token token;

    bool ret = tokenizer->peek(token);
    if(!ret)
        return NULL;
    
    std::stack<Node*> nodes;
    while(1)
    {
        switch(token.type)
        {
            case TokenType::Integer:
                nodes.push(nodeFactory->createInteger(token.c_str()));
                break;
            case TokenType::Float:
                nodes.push(nodeFactory->createFloat(token.c_str()));
                break;
            case TokenType::String:
                nodes.push(nodeFactory->createString(token.c_str()));
                break;
            case TokenType::Operator:
                nodes.push(nodeFactory->createString(token.c_str()));
                break;
            case TokenType::Comment:
                break;

        }
    }

    if(token.type == TokenType::Operator && symbolRegistry->isPrefixOperator(token.c_str()))
    {
    }

}
