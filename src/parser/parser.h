#ifndef PARSER_H
#define PARSER_H

class Tokenizer;
class Node;
class Parser
{
public:
    Parser();
    ~Parser();
public:
    Node* parse(const wchar_t* code);
private:
    
    Tokenizer* tokenizer;
};


#endif//PARSER_H