#ifndef FOR_IN_LOOP_H
#define FOR_IN_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
class CodeBlock;
class ForInLoop : public Statement
{
public:
    ForInLoop();
    ~ForInLoop();
public:
    virtual void serialize(std::wostream& out);
public:
    void setLoopVars(Pattern* val);
    Pattern* getLoopVars();
    
    void setContainer(Expression* vla);
    Expression* getContainer();
    
    void setCodeBlock(CodeBlock* val);
    CodeBlock* getCodeBlock();
    
private:
    Pattern* loopVars;
    Expression* container;
    CodeBlock* codeBlock;
};

SWIFT_NS_END

#endif//FOR_IN_LOOP_H
