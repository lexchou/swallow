#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"
#include "tokenizer/Token.h"
#include <string>
#include "ast/ast-decl.h"

SWIFT_NS_BEGIN

class Tokenizer;
class NodeFactory;
class CompilerResults;

class Parser
{
    friend struct Flags;
public:
    Parser(NodeFactory* nodeFactory, CompilerResults* compilerResults);
    ~Parser();
public:
    NodePtr parseStatement(const wchar_t* code);
    ProgramPtr parse(const wchar_t* code);
    bool parse(const wchar_t* code, const ProgramPtr& program);
    void setFileName(const wchar_t* fileName);
    void setFunctionName(const wchar_t* function);
private:
    TypeNodePtr parseType();
    TypeNodePtr parseTypeAnnotation();
    
    TupleTypePtr parseTupleType();
    TypeIdentifierPtr parseTypeIdentifier();
    ProtocolCompositionPtr parseProtocolComposition();
    GenericParametersDefPtr parseGenericParametersDef();
private://statement
    StatementPtr parseStatement();
    StatementPtr parseLoopStatement();
    StatementPtr parseForLoop();
    
    StatementPtr parseForIn();
    StatementPtr parseForStatement();
    
    StatementPtr parseWhileLoop();
    StatementPtr parseDoLoop();
    IfStatementPtr parseIf();
    StatementPtr parseSwitch();
    void parseSwitchStatements(const CaseStatementPtr& case_);
    StatementPtr parseBreak();
    StatementPtr parseContinue();
    StatementPtr parseFallthrough();
    StatementPtr parseReturn();
    StatementPtr parseLabeledStatement();
    ExpressionPtr parseConditionExpression();
    
    CodeBlockPtr parseCodeBlock();
private://pattern
    PatternPtr parsePattern();
    PatternPtr parseTuple();
    PatternPtr parseEnumPattern();
    PatternPtr parseTypeCastingPattern();
private://Attribute
    void parseAttributes(std::vector<AttributePtr>& attributes);
    AttributePtr parseAttribute();
    void parseBalancedToken(const AttributePtr& attr);
    void parseBalancedTokens(const AttributePtr& attr, const wchar_t* end);
    
private://declaration
    DeclarationPtr parseDeclaration();
    int parseDeclarationSpecifiers();
    void verifyDeclarationSpecifiers(const Token& token, int actualSpecifiers, int expectedSpecifiers);
    DeclarationPtr parseImport(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseLet(const std::vector<AttributePtr>& attrs, int specifiers);
    DeclarationPtr parseVar(const std::vector<AttributePtr>& attrs, int specifiers);
    ValueBindingPtr parseVariableDeclaration();
    void parseWillSetDidSetBlock(const ComputedPropertyPtr& property);
    void parseWillSetClause(const ComputedPropertyPtr& property, bool opt);
    void parseDidSetClause(const ComputedPropertyPtr& property, bool opt);
    
    std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > parseGetterSetterBlock();
    std::pair<std::wstring, CodeBlockPtr> parseSetterClause();
    std::pair<CodeBlockPtr, CodeBlockPtr> parseGetterSetterKeywordBlock();
    
    DeclarationPtr parseTypealias(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseFunc(const std::vector<AttributePtr>& attrs, int specifiers);
    ParametersPtr parseParameterClause();

    DeclarationPtr parseEnum(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseRawValueEnum(const std::vector<AttributePtr>& attrs, const std::wstring& name, const TypeIdentifierPtr& baseType);
    DeclarationPtr parseUnionEnum(const std::vector<AttributePtr>& attrs, const std::wstring& name);

    
    DeclarationPtr parseStruct(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseClass(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseProtocol(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseInit(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseDeinit(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseExtension(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseSubscript(const std::vector<AttributePtr>& attrs);
    DeclarationPtr parseOperator(const std::vector<AttributePtr>& attrs);
private://expression
    ExpressionPtr parseFloat();
    ExpressionPtr parseInteger();
    ExpressionPtr parseString();
    ExpressionPtr parseExpression();
    ExpressionPtr parseBinaryExpression(const ExpressionPtr& lhs);
    ExpressionPtr parsePrefixExpression();
    ExpressionPtr parsePostfixExpression();
    FunctionCallPtr parseFunctionCallExpression();
    ExpressionPtr parsePrimaryExpression();
    ExpressionPtr parseLiteralExpression();
    ParenthesizedExpressionPtr parseParenthesizedExpression();
    void parseExpressionItem(const ParenthesizedExpressionPtr& parent);
    ExpressionPtr parseLiteral();
    
    ExpressionPtr parseSelfExpression();
    ExpressionPtr parseSuperExpression();
    IdentifierPtr parseIdentifier();
    ClosurePtr parseClosureExpression();
    bool isGenericArgument();
    GenericArgumentDefPtr parseGenericArgumentDef();
    
    
    std::pair<ExpressionPtr, ExpressionPtr> parseDictionaryLiteralItem();
private:
    /**
     * Read next token from tokenizer, throw exception if EOF reached.
     */
    void expect_next(Token& token);
    /**
     * Restore the position of tokenizer to specified token
     */
    void restore(Token& token);
    /**
     * Read next token from tokenizer, return false if EOF reached.
     */
    bool next(Token& token);
    /**
     * Peek next token from tokenizer, return false if EOF reached.
     */
    bool peek(Token& token);
    /**
     * Check if the following token is an identifier, throw exception if not matched
     */
    void expect_identifier(Token& token);
    /**
     * Check if the following token is the specified one, throw exception if not matched
     */
    void expect(const wchar_t* token);
    void expect(const wchar_t* token, Token&);
    /**
     * Check if the following token is the specified keyword, throw exception if not matched
     */
    void expect(Keyword::T keyword);
    void expect(Keyword::T keyword, Token&);

    /**
     * Throw an exception with the unexpected token
     */
    void unexpected(const Token& token);

    /**
     * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
     */
    bool match(const wchar_t* token);
    bool match(Keyword::T keyword);
    bool match(const wchar_t* token, Token&);
    bool match(Keyword::T keyword, Token&);
    /**
     * Check if the following token is an identifier(keywords included), consume the token and return true if matched or rollback and return false
     */
    bool match_identifier(Token& token);
    
    /**
     * Return true if the next token is the specified one, no token will be consumed
     */
    bool predicate(const wchar_t* token);

    void tassert(Token& token, bool cond, int errorCode, const std::wstring& s = std::wstring());
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    CompilerResults* compilerResults;
    std::wstring fileName;
    int fileHash;
    std::wstring functionName;
    int flags;

};



SWIFT_NS_END

#endif//PARSER_H
