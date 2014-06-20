#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"

Parser::Parser()
{
    tokenizer = new Tokenizer(NULL);
}
Parser::~Parser()
{
    delete tokenizer;
}

Node* Parser::parse(const wchar_t* code)
{
    tokenizer.set(code);
    
}
