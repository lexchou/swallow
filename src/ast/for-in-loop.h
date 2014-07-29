#ifndef FOR_IN_LOOP_H
#define FOR_IN_LOOP_H
#include "statement.h"
#include <string>
#include "ast-decl.h"

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
    virtual void accept(NodeVisitor* visitor);
public:
    void setLoopVars(const PatternPtr& val);
    PatternPtr getLoopVars();
    
    void setContainer(const ExpressionPtr& vla);
    ExpressionPtr getContainer();
    
    void setCodeBlock(const CodeBlockPtr& val);
    CodeBlockPtr getCodeBlock();
    
private:
    PatternPtr loopVars;
    ExpressionPtr container;
    CodeBlockPtr codeBlock;
};

SWIFT_NS_END

#endif//FOR_IN_LOOP_H
