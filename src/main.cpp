#include "tokenizer/tokens.h"
#include "tokenizer/tokenizer.h"
#include <iostream>
int main(int argc, char** argv)
{
    Tokenizer tokenizer(L"println( ++ )");
    Token token;
    while(tokenizer.next(token))
    {
        token.token.push_back(0);
        const wchar_t *str = &token.token.front();
        std::wcout<<str<<std::endl;
    }
    
}