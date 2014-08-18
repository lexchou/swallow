#include "Parser.h"
#include "parser_details.h"
#include "tokenizer/Tokenizer.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
#include "swift_errors.h"

using namespace Swift;

ExpressionPtr Parser::parseFloat()
{
    Token token;
    expect_next(token);
    FloatLiteralPtr ret = nodeFactory->createFloat(token.state);
    ret->valueAsString = token.token;
    ret->value = token.number.dvalue;
    return ret;
}
ExpressionPtr Parser::parseInteger()
{
    Token token;
    expect_next(token);
    IntegerLiteralPtr ret = nodeFactory->createInteger(token.state);
    ret->valueAsString = token.token;
    ret->value = token.number.value;
    ret->dvalue = token.number.dvalue;
    ret->isFloat = token.type == TokenType::Float;
    return ret;
}
ExpressionPtr Parser::parseString()
{
    Token token;
    expect_next(token);
    StringLiteralPtr ret = nodeFactory->createString(token.state);
    ret->value = token.token;
    return ret;
}
/*
 GRAMMAR OF A PREFIX EXPRESSION
 prefix-expression → prefix-operator opt postfix-expression
 prefix-expression → in-out-expression
 in-out-expression → & identifier
 */
ExpressionPtr Parser::parsePrefixExpression()
{
    Token token;
    if(match(L"&"))
    {
        //in-out-expression → & identifier
        expect_identifier(token);
        IdentifierPtr identifier = nodeFactory->createIdentifier(token.state);
        identifier->setIdentifier(token.token);
        InOutParameterPtr ret = nodeFactory->createInOutParameter(token.state);
        ret->setOperand(identifier);
        return ret;
    }
    
    expect_next(token);
    //if(symbolRegistry->isPrefixOperator(token.token))
    if(token.type == TokenType::Operator && token.operators.type == OperatorType::PrefixUnary)
    {
        ExpressionPtr postfixExpression = parsePostfixExpression();
        UnaryOperatorPtr op = nodeFactory->createUnary(token.state);
        op->setOperator(token.token);
        op->setOperatorType(token.operators.type);
        op->setOperand(postfixExpression);
        return op;
    }
    restore(token);
    ExpressionPtr ret = parsePostfixExpression();
    return ret;
}
/*
 GRAMMAR OF A POSTFIX EXPRESSION
 postfix-expression → primary-expression
 postfix-expression → postfix-expression postfix-operator
 postfix-expression → function-call-expression
 postfix-expression → initializer-expression
 postfix-expression → explicit-member-expression
 postfix-expression → postfix-self-expression
 postfix-expression → dynamic-type-expression
 postfix-expression → subscript-expression
 postfix-expression → forced-value-expression
 postfix-expression → optional-chaining-expression
 */
ExpressionPtr Parser::parsePostfixExpression()
{
    Token token;
    // postfix-expression → primary-expression
    ExpressionPtr ret =  parsePrimaryExpression();
    while(peek(token))
    {
        if(token.type == TokenType::Operator)
        {
            if(token == L".")
            {
                expect_next(token);//skip .
                expect_next(token);//read identifier
                tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_INIT_SELF_DYNAMICTYPE_IDENTIFIER, token.token);
                
                // postfix-expression → initializer-expression
                if(token == L"init")
                {
                    InitializerReferencePtr r = nodeFactory->createInitializerReference(token.state);
                    r->setExpression(ret);
                    ret = r;
                    continue;
                }
                // postfix-expression → postfix-self-expression
                if(token.identifier.keyword == Keyword::Self)
                {
                    SelfExpressionPtr r = nodeFactory->createSelfExpression(token.state);
                    r->setExpression(ret);
                    ret = r;
                    continue;
                }
                // postfix-expression → dynamic-type-expression
                if(token.identifier.keyword == Keyword::DynamicType)
                {
                    DynamicTypePtr r = nodeFactory->createDynamicType(token.state);
                    r->setExpression(ret);
                    ret = r;
                    continue;
                }
                tassert(token, token.identifier.keyword == Keyword::_, Errors::E_EXPECT_IDENTIFIER, token.token);
                // postfix-expression → explicit-member-expression
                IdentifierPtr field = nodeFactory->createIdentifier(token.state);
                field->setIdentifier(token.token);
                MemberAccessPtr access = nodeFactory->createMemberAccess(token.state);
                access->setSelf(ret);
                access->setField(field);
                ret = access;
                continue;
            }
            if(token == L"!")
            {
                // postfix-expression → forced-value-expression
                expect_next(token);
                ForcedValuePtr r = nodeFactory->createForcedValue(token.state);
                r->setExpression(ret);
                ret = r;
                continue;
            }
            // postfix-expression → postfix-expression postfix-operator
            if(token.type == TokenType::Operator && token.operators.type == OperatorType::PostfixUnary)//symbolRegistry->isPostfixOperator(token.token))
            {
                expect_next(token);
                UnaryOperatorPtr postfix = nodeFactory->createUnary(token.state);
                postfix->setOperator(token.token);
                postfix->setOperatorType(OperatorType::PostfixUnary);
                postfix->setOperand(ret);
                ret = postfix;
                continue;
            }
            break;
        }
        if(token.type == TokenType::Optional && token.operators.type == OperatorType::PostfixUnary)
        {
            //? used as post unary operator will treated as optional chaining expression, not ternary expression
            // postfix-expression → optional-chaining-expression
            expect_next(token);
            OptionalChainingPtr r = nodeFactory->createOptionalChaining(token.state);
            r->setExpression(ret);
            ret = r;
            continue;
        }
        // postfix-expression → function-call-expression
        if(token.type == TokenType::OpenParen || token.type == TokenType::OpenBrace)
        {
            bool suppressTrailingClosure = (this->flags & SUPPRESS_TRAILING_CLOSURE) != 0;
            if(!(token.type == TokenType::OpenBrace && suppressTrailingClosure))
            {
                FunctionCallPtr call = parseFunctionCallExpression();
                call->setFunction(ret);
                ret = call;
                continue;
            }
        }
        // postfix-expression → subscript-expression
        if(token.type == TokenType::OpenBracket)
        {
            // subscript-expression → postfix-expression[expression-list]
            match(L"[", token);
            ExpressionPtr expr = parseExpression();
            SubscriptAccessPtr subscript = nodeFactory->createSubscriptAccess(token.state);
            subscript->setSelf(ret);
            subscript->addIndex(expr);
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
                expr = parseExpression();
                subscript->addIndex(expr);
            }
            match(L"]");
            ret = subscript;
            continue;
        }
        break;
    }
    return ret;
}

/*
 GRAMMAR OF A FUNCTION CALL EXPRESSION
 
 ‌ function-call-expression → postfix-expression parenthesized-expression
 ‌ function-call-expression → postfix-expression parenthesized-expression opt trailing-closure
 ‌ trailing-closure → closure-expression
 */
FunctionCallPtr Parser::parseFunctionCallExpression()
{
    FunctionCallPtr ret = nodeFactory->createFunctionCall(tokenizer->save());
    if(predicate(L"("))
    {
        ParenthesizedExpressionPtr p = this->parseParenthesizedExpression();
        ret->setArguments(p);
    }
    bool suppressTrailingClosure = (this->flags & SUPPRESS_TRAILING_CLOSURE) != 0;
    if(!suppressTrailingClosure && predicate(L"{"))
    {
        ClosurePtr closure = parseClosureExpression();
        ret->setTrailingClosure(closure);
    }
    return ret;
}


/*
 GRAMMAR OF A PRIMARY EXPRESSION
 primary-expression → identifier generic-argument-clauseopt
 primary-expression → literal-expression
 primary-expression → self-expression
 primary-expression → superclass-expression
 primary-expression → closure-expression
 primary-expression → parenthesized-expression
 primary-expression → implicit-member-expression
 primary-expression → wildcard-expression
 */
ExpressionPtr Parser::parsePrimaryExpression()
{
    Token token;
    peek(token);
    if(token.type == TokenType::Identifier)
    {
        //primary-expression → identifier generic-argument-clauseopt
        switch(token.identifier.keyword)
        {
            case Keyword::File:
            case Keyword::Line:
            case Keyword::Column:
            case Keyword::Function:
                return parseLiteralExpression();
            case Keyword::Self:
                // primary-expression → self-expression
                return parseSelfExpression();
            case Keyword::Super:
                // primary-expression → superclass-expression
                return parseSuperExpression();
            default:
                break;
        }
        IdentifierPtr identifier = parseIdentifier();
        return identifier;
    }
    // primary-expression → literal-expression
    switch(token.type)
    {
        case TokenType::Integer:
        case TokenType::Float:
        case TokenType::String:
        case TokenType::OpenBracket:
            return parseLiteralExpression();
        default:
            break;
    }
    
    // primary-expression → parenthesized-expression
    if(token.type == TokenType::OpenParen)
        return parseParenthesizedExpression();
    // primary-expression → wildcard-expression
    if(token == L"_")
    {
        expect_next(token);
        IdentifierPtr id = nodeFactory->createIdentifier(token.state);
        id->setIdentifier(L"_");
        return id;
    }
    
    // primary-expression → closure-expression
    if(token.type == TokenType::OpenBrace)
    {
        return parseClosureExpression();
    }
    
    // primary-expression → implicit-member-expression
    if(token == L".")
    {
        expect_next(token);
        expect_identifier(token);
        IdentifierPtr field = nodeFactory->createIdentifier(token.state);
        field->setIdentifier(token.token);
        MemberAccessPtr ret = nodeFactory->createMemberAccess(token.state);
        ret->setField(field);
        return ret;
    }
    return NULL;
}

bool Parser::isGenericArgument()
{
    /*
    if(!(this->flags & ENABLE_GENERIC))
        return false;
    */
    Token token, t;
    if(!match(L"<", token))
    {
        restore(token);
        return false;
    }
    int nestedLevel = 1;
    bool ret = false;
    while(true)
    {
        if(!next(t))
            break;
        if(t == L"<")
            nestedLevel ++;
        else if(t == L">")
        {
            nestedLevel --;
            if(nestedLevel == 0)
            {
                ret = true;
                break;
            }
        }
        else if(t.type == TokenType::OpenBrace)
            break;
    }
    restore(token);
    return ret;
}
/*
  GRAMMAR OF A GENERIC ARGUMENT CLAUSE
 
 ‌ generic-argument-clause → <generic-argument-list>
 ‌ generic-argument-list → generic-argument | generic-argument,generic-argument-list
 ‌ generic-argument → type
*/
GenericArgumentPtr Parser::parseGenericArgument()
{
    Token token;
    expect(L"<", token);
    GenericArgumentPtr ret = nodeFactory->createGenericArgument(token.state);
    do
    {
        TypeNodePtr argument = parseType();
        ret->addArgument(argument);
    } while(match(L","));
    expect(L">");
    return ret;
}



/*
 GRAMMAR OF A PARENTHESIZED EXPRESSION
 
 ‌ parenthesized-expression → (expression-element-list opt)
 ‌ expression-element-list → expression-element | expression-element,expression-element-list
 ‌ expression-element → expression | identifier:expression
 */
ParenthesizedExpressionPtr Parser::parseParenthesizedExpression()
{
    Token token;
    match(L"(", token);
    ParenthesizedExpressionPtr ret = nodeFactory->createParenthesizedExpression(token.state);
    if(!predicate(L")"))
    {
        parseExpressionItem(ret);
    }
    while(match(L","))
    {
        if(predicate(L")"))
            break;
        parseExpressionItem(ret);
    }
    match(L")");
    return ret;
}

void Parser::parseExpressionItem(const ParenthesizedExpressionPtr& parent)
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_ && match(L":"))
    {
        //identifier:expression
        ExpressionPtr expr = parseExpression();
        parent->append(token.token, expr);
        return;
    }
    //rollback and parse exception
    restore(token);
    ExpressionPtr expr = parseExpression();
    parent->append(expr);
}

/*
 GRAMMAR OF A SELF EXPRESSION
 
 ‌ self-expression → self
 ‌ self-expression → self.identifier
 ‌ self-expression → self[expression]
 ‌ self-expression → self.init
 */
ExpressionPtr Parser::parseSelfExpression()
{
    Token token;
    expect(Keyword::Self);
    expect_next(token);
    IdentifierPtr self = nodeFactory->createIdentifier(token.state);
    self->setIdentifier(L"self");
    if(token == L".")
    {
        expect_next(token);
        tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER, token.token);
        if(token.identifier.keyword != Keyword::_ && token.identifier.keyword != Keyword::Init)
            unexpected(token);
        IdentifierPtr field = nodeFactory->createIdentifier(token.state);
        field->setIdentifier(token.token);
        MemberAccessPtr ret = nodeFactory->createMemberAccess(token.state);
        ret->setField(field);
        ret->setSelf(self);
        return ret;
    }
    else if(token == L"[")
    {
        ExpressionPtr expr = this->parseExpression();
        SubscriptAccessPtr sub = nodeFactory->createSubscriptAccess(token.state);
        sub->setSelf(self);
        sub->addIndex(expr);
        return sub;
    }
    else
    {
        restore(token);
        return self;
    }
}
/*
 superclass-expression → superclass-method-expression superclass-subscript-expression  superclass-initializer-expression
 ‌ superclass-method-expression → super.identifier
 ‌ superclass-subscript-expression → super[expression]
 ‌ superclass-initializer-expression → super.init
 */
ExpressionPtr Parser::parseSuperExpression()
{
    Token token;
    expect(Keyword::Super, token);
    IdentifierPtr super = nodeFactory->createIdentifier(token.state);
    super->setIdentifier(L"super");
    expect_next(token);
    if(token == L".")
    {
        expect_next(token);
        if(token.type != TokenType::Identifier)
            unexpected(token);
        if(token.identifier.keyword != Keyword::_ && token.identifier.keyword != Keyword::Init)
            unexpected(token);
        IdentifierPtr field = nodeFactory->createIdentifier(token.state);
        field->setIdentifier(token.token);
        MemberAccessPtr ret = nodeFactory->createMemberAccess(token.state);
        ret->setSelf(super);
        ret->setField(field);
        return ret;
    }
    else if(token == L"[")
    {
        ExpressionPtr expr = this->parseExpression();
        expect(L"]", token);
        SubscriptAccessPtr sub = nodeFactory->createSubscriptAccess(token.state);
        sub->setSelf(super);
        sub->addIndex(expr);
        return sub;
    }
    unexpected(token);
    return NULL;
}

IdentifierPtr Parser::parseIdentifier()
{
    Token token;
    expect_identifier(token);
    IdentifierPtr ret = nodeFactory->createIdentifier(token.state);
    ret->setIdentifier(token.token);
    
    if(isGenericArgument())
    {
        GenericArgumentPtr arg = parseGenericArgument();
        ret->setGenericArgument(arg);
    }
    return ret;
}

/*
 literal-expression → literal
 literal-expression → array-literal dictionary-literal
 literal-expression → __FILE__ __LINE__ __COLUMN__ __FUNCTION__
 array-literal → [ array-literal-items opt ]
 array-literal-items → array-literal-item ,opt
 array-literal-item , array-literal-items
 array-literal-item → expression
 dictionary-literal → [ dictionary-literal-items ] [ : ]
 dictionary-literal-item → dictionary-literal-item, opt dictionary-literal-item, dictionarya-literal-items
 dictionary-literal-item → expression : expression
 */
ExpressionPtr Parser::parseLiteralExpression()
{
    Token token;
    expect_next(token);
    // literal-expression → literal
    if(token.type == TokenType::Integer || token.type == TokenType::Float || token.type == TokenType::String)
    {
        restore(token);
        return parseLiteral();
    }
    // literal-expression → array-literal dictionary-literal
    
    if(token.type == TokenType::OpenBracket)
    {
        expect_next(token);
        if(token.type == TokenType::CloseBracket)
        {
            //[] detected, empty array
            return nodeFactory->createArrayLiteral(token.state);
        }
        if(token.type == TokenType::Colon)
        {
            //[: detected, empty dictionary
            expect(L"]", token);
            return nodeFactory->createDictionaryLiteral(token.state);
        }
        restore(token);
        //check if there's a colon after an expression
        
        ExpressionPtr tmp = parseExpression();
        tassert(token, tmp != NULL, Errors::E_EXPECT_EXPRESSION, token.token);
        peek(token);
        if(token.type == TokenType::Comma || token.type == TokenType::CloseBracket)//array
        {
            ArrayLiteralPtr array = nodeFactory->createArrayLiteral(token.state);
            array->push(ExpressionPtr(tmp));
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
                tmp = parseExpression();
                array->push(ExpressionPtr(tmp));
            }
            expect(L"]");
            return array;
            
        }
        else if(token.type == TokenType::Colon)//dictionary
        {
            match(L":");
            DictionaryLiteralPtr dict = nodeFactory->createDictionaryLiteral(token.state);
            ExpressionPtr key = tmp;
            ExpressionPtr value = parseExpression();
            tassert(token, value != NULL, Errors::E_EXPECT_EXPRESSION, token.token);
            dict->insert(key, value);
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
                key = parseExpression();
                expect(L":");
                value = parseExpression();
                dict->insert(key, value);
            }
            expect(L"]");
            return dict;
        }
        unexpected(token);
    }
    
    if(token.type == TokenType::Identifier)
    {
        switch(token.identifier.keyword)
        {
            case Keyword::File:
            {
                CompileConstantPtr c = nodeFactory->createCompilecConstant(token.state);
                c->setName(L"__FILE__");
                c->setValue(fileName);
                return c;
            }
            case Keyword::Line:
            {
                std::wstringstream ss;
                ss<<token.state.column;
                CompileConstantPtr c = nodeFactory->createCompilecConstant(token.state);
                c->setName(L"__LINE__");
                c->setValue(ss.str());
                return c;
            }
            case Keyword::Column:
            {
                std::wstringstream ss;
                ss<<token.state.line;
                CompileConstantPtr c = nodeFactory->createCompilecConstant(token.state);
                c->setName(L"__COLUMN__");
                c->setValue(ss.str());
                return c;
            }
            case Keyword::Function:
            {
                CompileConstantPtr c = nodeFactory->createCompilecConstant(token.state);
                c->setName(L"__FUNCTION__");
                c->setValue(functionName);
                return c;
            }
            default:
                break;
        }
    }
    
    unexpected(token);
    return NULL;
}
ExpressionPtr Parser::parseLiteral()
{
    Token token;
    expect_next(token);
    switch(token.type)
    {
        case TokenType::Integer:
        {
            IntegerLiteralPtr i = nodeFactory->createInteger(token.state);
            i->valueAsString = token.token;
            i->value = token.number.value;
            return i;
        }
        case TokenType::String:
        {
            StringLiteralPtr s = nodeFactory->createString(token.state);
            s->value = token.token;
            return s;
        }
        case TokenType::Float:
        {
            FloatLiteralPtr f = nodeFactory->createFloat(token.state);
            f->valueAsString = token.token;
            f->value = token.number.dvalue;
            return f;
        }
        default:
            unexpected(token);
            break;
    }
    return NULL;
}
std::pair<ExpressionPtr, ExpressionPtr> Parser::parseDictionaryLiteralItem()
{
    ExpressionPtr key = parseExpression();
    expect(L":");
    ExpressionPtr value = parseExpression();
    return std::make_pair(key, value);
}

static inline bool isBinaryExpr(const Token& token)
{
    if(token.type == TokenType::Identifier)
    {
        return token.identifier.keyword == Keyword::Is || token.identifier.keyword == Keyword::As;
    }
    if(token.type == TokenType::Optional && token.operators.type == OperatorType::InfixBinary)
        return true;
    if(token.type != TokenType::Operator)
        return false;
    if(token.operators.type == OperatorType::InfixBinary)
        return true;
    if(token == L"=")
        return true;
    if(token == L"?")
        return true;
    return false;
}
/**
 * Rotate the AST tree if required
 */
static int rotateRequired(const OperatorPtr& lhs, const OperatorPtr& rhs)
{
    if(lhs->getPrecedence() == rhs->getPrecedence())
    {
        //only right associativity is required because it's left associativity
        return lhs->getAssociativity() == Associativity::Right;
    }
    return lhs->getPrecedence() < rhs->getPrecedence();
}
/**
 * Sort the AST tree by precedence, if the precedence is the same, sort by associativity
 */
static OperatorPtr sortExpression(OperatorPtr op)
{
    OperatorPtr root = op;
    OperatorPtr c;
    if(op->numChildren() != 2)
        return op;
    NodePtr left = op->get(0);
    NodePtr right = op->get(1);
    if(left && (c = std::dynamic_pointer_cast<Operator>(left)) && c->numChildren() == 2)
    {
        left = c = sortExpression(c);
        if(rotateRequired(c, root))
        {
            //make c the root
            NodePtr r = c->get(1);
            c->set(1, root);
            root->set(0, r);
            root = c;
            //sort new right child
            NodePtr r2 = sortExpression(std::static_pointer_cast<Operator>(c->get(1)));
            c->set(1, r2);
            return root;
        }
    }
    //sort old right child
    c = std::dynamic_pointer_cast<Operator>(right);
    if(c != NULL)
        sortExpression(c);
    return root;
}
/*
 expression → prefix-expression binary-expressions opt
 ‌ expression-list → expression | expression,expression-list
 */
ExpressionPtr Parser::parseExpression()
{
    Token token;
    ExpressionPtr ret = parsePrefixExpression();
    peek(token);
    
    //‌ binary-expressions → binary-expressionbinary-expressionsopt
    if(isBinaryExpr(token))
    {
        ret = parseBinaryExpression(ret);
        for(bool succ = peek(token); succ && isBinaryExpr(token); succ = peek(token))
        {
            ret = parseBinaryExpression(ret);
        }
    }
    if(OperatorPtr op = std::dynamic_pointer_cast<Operator>(ret))
    {
        //sort the tree by associativity and precedence
        ret = sortExpression(op);
    }
    return ret;
}
/*
 GRAMMAR OF A BINARY EXPRESSION
 
 ‌ binary-expression → binary-operatorprefix-expression
 ‌ binary-expression → assignment-operatorprefix-expression
 ‌ binary-expression → conditional-operatorprefix-expression
 ‌ binary-expression → type-casting-operator
 */
ExpressionPtr Parser::parseBinaryExpression(const ExpressionPtr& lhs)
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier)
    {
        if(token.identifier.keyword == Keyword::Is)
        {
            TypeNodePtr typeNode = parseType();
            TypeCheckPtr ret = nodeFactory->createTypeCheck(token.state);
            ret->setLHS(lhs);
            ret->setDeclaredType(typeNode);
            return ret;
        }
        else if(token.identifier.keyword == Keyword::As)
        {
            bool optional = false;
            Token t;
            if(match(L"?", t))
            {
                if(t.operators.type == OperatorType::PostfixUnary)
                {
                    optional = true;
                }
                else
                    restore(t);
            }
            
            TypeNodePtr typeNode = parseType();
            
            TypeCastPtr ret = nodeFactory->createTypeCast(token.state);
            ret->setLHS(lhs);
            ret->setDeclaredType(typeNode);
            ret->setOptional(optional);
            return ret;
        }
    }
    if(token.type == TokenType::Operator)
    {
        if(token == L"=")
        {
            ExpressionPtr rhs = parsePrefixExpression();
            AssignmentPtr ret = nodeFactory->createAssignment(token.state);
            ret->setLHS(lhs);
            ret->setRHS(rhs);
            return ret;
        }
        
        if(token.operators.type == OperatorType::InfixBinary)
        {
            //OperatorInfo* op = symbolRegistry->getOperator(token.token);
            //tassert(token, op != NULL, Errors::E_UNDEFINED_INFIX_OPERATOR, token.token);
            ExpressionPtr rhs = parsePrefixExpression();
            //int precedence = op->precedence.infix > 0 ? op->precedence.infix : 100;
            BinaryOperatorPtr ret = nodeFactory->createBinary(token.state);
            ret->setOperator(token.token);
            //ret->setAssociativity(op->associativity);
            //ret->setPrecedence(precedence);
            ret->setLHS(lhs);
            ret->setRHS(rhs);
            return ret;
        }
    }
    if(token.type == TokenType::Optional)
    {
        // binary-expression → conditional-operator prefix-expression
        ExpressionPtr expr = parseExpression();
        expect(L":");
        ExpressionPtr expr2 = parsePrefixExpression();
        ConditionalOperatorPtr ret = nodeFactory->createConditionalOperator(token.state);
        ret->setCondition(lhs);
        ret->setTrueExpression(expr);
        ret->setFalseExpression(expr2);
        return ret;
    }
    unexpected(token);
    return NULL;
}
/*
  GRAMMAR OF A CLOSURE EXPRESSION
 
 ‌ closure-expression → {closure-signature opt statements}
 ‌ closure-signature → parameter-clause function-result opt in
 ‌ closure-signature → identifier-list function-result opt in
 ‌ closure-signature → capture-list parameter-clause function-result opt in
 ‌ closure-signature → capture-list identifier-list function-result opt in
 ‌ closure-signature → capture-list in
 ‌ capture-list → [capture-specifier expression]
 ‌ capture-specifier → weak  unowned  unowned(safe) unowned(unsafe)
*/
ClosurePtr Parser::parseClosureExpression()
{
    Token token;
    expect(L"{", token);
    ClosurePtr ret = nodeFactory->createClosure(token.state);
    TokenizerState s = tokenizer->save();
    bool hasSignature = false;
    //look for keyword "in"
    while(next(token))
    {
        Keyword::T keyword = token.getKeyword();
        if(keyword == Keyword::In)
        {
            hasSignature = true;
            break;
        }
        if(token == L"}")
            break;
    }
    tokenizer->restore(s);
    if(hasSignature)
    {
        if(match(L"["))
        {
            //read capture-list
            expect_next(token);
            Keyword::T k = token.getKeyword();
            tassert(token, k == Keyword::Weak || k == Keyword::Unowned || k == Keyword::Unowned_safe || k == Keyword::Unowned_unsafe, Errors::E_EXPECT_CAPTURE_SPECIFIER, token.token);
            switch(token.getKeyword())
            {
                case Keyword::Weak:
                    ret->setCaptureSpecifier(Closure::Weak);
                    break;
                case Keyword::Unowned:
                    ret->setCaptureSpecifier(Closure::Unowned);
                    break;
                case Keyword::Unowned_safe:
                    ret->setCaptureSpecifier(Closure::Unowned_Safe);
                    break;
                case Keyword::Unowned_unsafe:
                    ret->setCaptureSpecifier(Closure::Unowned_Unsafe);
                    break;
                default:
                    unexpected(token);
                    break;
            }
            ExpressionPtr capture = parseExpression();
            ret->setCapture(capture);
            expect(L"]");
        }
        if(predicate(L"("))
        {
            ParametersPtr params = parseParameterClause();
            ret->setParameters(params);
        }
        else
        {
            ParametersPtr params = nodeFactory->createParameters(tokenizer->save());
            do
            {
                expect_identifier(token);
                ParameterPtr param = nodeFactory->createParameter(token.state);
                param->setLocalName(token.token);
                params->addParameter(param);
            } while (match(L","));
            ret->setParameters(params);
        }
        if(match(L"->"))
        {
            TypeNodePtr t = parseType();
            ret->setReturnType(t);
        }
        expect(Keyword::In);
    }
    while(!predicate(L"}"))
    {
        StatementPtr s = parseStatement();
        ret->addStatement(s);
    }
    expect(L"}");
    return ret;
}

