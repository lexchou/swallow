#include "node-factory.h"
#include "ast/ast.h"
USE_SWIFT_NS;

CommentNode* NodeFactory::createComment(const std::wstring& comment)
{
    return new CommentNode(comment);
}
BooleanLiteral* NodeFactory::createBoolean(bool value)
{
    return new BooleanLiteral(value);
}
IntegerLiteral* NodeFactory::createInteger(const std::wstring& value)
{
    return new IntegerLiteral(value);
}
FloatLiteral* NodeFactory::createFloat(const std::wstring& value)
{
    return new FloatLiteral(value);
}
StringLiteral* NodeFactory::createString(const std::wstring& value)
{
    return new StringLiteral(value);
}
UnaryOperator* NodeFactory::createUnary(const std::wstring& op, OperatorType::T type)
{
    UnaryOperator* ret = new UnaryOperator(op, type);
    return ret;
}
Identifier* NodeFactory::createIdentifier(const std::wstring& op)
{
    return new Identifier(op);
}
InOutParameter* NodeFactory::createInOutParameter(Identifier* identifier)
{
    InOutParameter* ret = new InOutParameter(identifier);
    return ret;
}
BinaryOperator* NodeFactory::createBinary(const std::wstring& op, Associativity::T associativity, int precedence)
{
    BinaryOperator* ret = new BinaryOperator(op, associativity, precedence);
    return ret;
}

ArrayLiteral* NodeFactory::createArrayLiteral()
{
    return new ArrayLiteral();
}
DictionaryLiteral* NodeFactory::createDictionaryLiteral()
{
    return new DictionaryLiteral();
}
CompileConstant* NodeFactory::createCompilecConstant(const std::wstring& name, const std::wstring& value)
{
    return new CompileConstant(name, value);
}

MemberAccess* NodeFactory::createMemberAccess(Expression* self, Identifier* field)
{
    return new MemberAccess(self, field);
}
SubscriptAccess* NodeFactory::createSubscriptAccess(Expression* self, Expression* index)
{
    return new SubscriptAccess(self, index);
}

TypeCheck* NodeFactory::createTypeCheck(Pattern* expr, TypeNode* type)
{
    return new TypeCheck(expr, type);
}
TypeCast* NodeFactory::createTypeCast(Expression* expr, TypeNode* type)
{
    return new TypeCast(expr, type);
}
Assignment* NodeFactory::createAssignment(Expression* lhs, Expression* rhs)
{
    return new Assignment(lhs, rhs);
}
ConditionalOperator* NodeFactory::createConditionalOperator(Expression* cond, Expression* trueExpr, Expression* falseExpr)
{
    return new ConditionalOperator(cond, trueExpr, falseExpr);
}
ParenthesizedExpression* NodeFactory::createParenthesizedExpression()
{
    return new ParenthesizedExpression();
}

InitializerReference* NodeFactory::createInitializerReference(Expression* expr)
{
    return new InitializerReference(expr);
}
SelfExpression* NodeFactory::createSelfExpression(Expression* expr)
{
    return new SelfExpression(expr);
}
DynamicType* NodeFactory::createDynamicType(Expression* expr)
{
    return new DynamicType(expr);
}
ForcedValue* NodeFactory::createForcedValue(Expression* expr)
{
    return new ForcedValue(expr);
}
OptionalChaining* NodeFactory::createOptionalChaining(Expression* expr)
{
    return new OptionalChaining(expr);
}
FunctionCall* NodeFactory::createFunctionCall()
{
    return new FunctionCall();
}


ForLoop* NodeFactory::createForLoop()
{
    return new ForLoop();
}
WhileLoop* NodeFactory::createWhileLoop()
{
    return new WhileLoop();
}
IfStatement* NodeFactory::createIf()
{
    return new IfStatement();
}
DoLoop* NodeFactory::createDoLoop()
{
    return new DoLoop();
}
SwitchCase* NodeFactory::createSwitch()
{
    return new SwitchCase();
}
CaseStatement* NodeFactory::createCase()
{
    return new CaseStatement();
}
BreakStatement* NodeFactory::createBreak()
{
    return new BreakStatement();
}
ContinueStatement* NodeFactory::createContinue()
{
    return new ContinueStatement();
}
FallthroughStatement* NodeFactory::createFallthrough()
{
    return new FallthroughStatement();
}
ReturnStatement* NodeFactory::createReturn()
{
    return new ReturnStatement();
}
LabeledStatement* NodeFactory::createLabel()
{
    return new LabeledStatement();
}
CodeBlock* NodeFactory::createCodeBlock()
{
    return new CodeBlock();
}
LetBinding* NodeFactory::createLetBinding()
{
    return new LetBinding();
}
VarBinding* NodeFactory::createVarBinding()
{
    return new VarBinding();
}
Tuple* NodeFactory::createTuple()
{
    return new Tuple();
}


FunctionType* NodeFactory::createFunctionType(TypeNode* argumentsType, TypeNode* retType)
{
    FunctionType* ret = new FunctionType();
    ret->setArgumentsType(argumentsType);
    ret->setReturnType(retType);
    return ret;
}
ArrayType* NodeFactory::createArrayType(TypeNode* innerType)
{
    ArrayType* ret = new ArrayType();
    ret->setInnerType(innerType);
    return ret;
}
OptionalType* NodeFactory::createOptionalType(TypeNode* innerType)
{
    OptionalType* ret = new OptionalType();
    ret->setInnerType(innerType);
    return ret;
}
ImplicitlyUnwrappedOptional* NodeFactory::createImplicitlyUnwrappedOptional(TypeNode* innerType)
{
    ImplicitlyUnwrappedOptional* ret = new ImplicitlyUnwrappedOptional();
    ret->setInnerType(innerType);
    return ret;
}
TypeIdentifier* NodeFactory::createTypeIdentifier(const std::wstring& typeName)
{
    TypeIdentifier* ret = new TypeIdentifier();
    ret->setName(typeName);
    return ret;
}
ProtocolComposition* NodeFactory::createProtocolComposition()
{
    return new ProtocolComposition();
}
TupleType* NodeFactory::createTupleType()
{
    return new TupleType();
}
Attribute* NodeFactory::createAttribute(const std::wstring& name)
{
    Attribute* ret = new Attribute();
    ret->setName(name);
    return ret;
}



Import* NodeFactory::createImport(const std::vector<Attribute*>& attrs)
{
    Import* ret = new Import();
    ret->setAttributes(attrs);
    return ret;
}
Constant* NodeFactory::createConstant(const std::vector<Attribute*>& attrs, int specifiers)
{
    Constant* ret = new Constant();
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}
Variables* NodeFactory::createVariables(const std::vector<Attribute*>& attrs, int specifiers)
{
    Variables* ret = new Variables();
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}

Variable* NodeFactory::createVariable()
{
    Variable* ret = new Variable();
    return ret;
}
TypeAlias* NodeFactory::createTypealias(const std::vector<Attribute*>& attrs)
{
    TypeAlias* ret = new TypeAlias();
    ret->setAttributes(attrs);
    return ret;
}
FunctionDef* NodeFactory::createFunction(const std::wstring& name, const std::vector<Attribute*>& attrs, int specifiers)
{
    FunctionDef* ret = new FunctionDef();
    ret->setAttributes(attrs);
    ret->setName(name);
    ret->setSpecifiers(specifiers);
    return ret;
}
Parameters* NodeFactory::createParameters()
{
    return new Parameters();
}
Parameter* NodeFactory::createParameter()
{
    return new Parameter();
}
EnumDef* NodeFactory::createEnum(const std::wstring& name, const std::vector<Attribute*>& attrs)
{
    EnumDef* ret = new EnumDef();
    ret->setAttributes(attrs);
    ret->setIdentifier(createTypeIdentifier(name));
    return ret;
}
StructDef* NodeFactory::createStruct(const std::wstring& name, const std::vector<Attribute*>& attrs)
{
    StructDef* ret = new StructDef();
    ret->setIdentifier(createTypeIdentifier(name));
    ret->setAttributes(attrs);
    return ret;
}
ClassDef* NodeFactory::createClass(const std::wstring& name, const std::vector<Attribute*>& attrs)
{
    ClassDef* ret = new ClassDef();
    ret->setIdentifier(createTypeIdentifier(name));
    ret->setAttributes(attrs);
    return ret;
}
ProtocolDef* NodeFactory::createProtocol(const std::wstring& name, const std::vector<Attribute*>& attrs)
{
    ProtocolDef* ret = new ProtocolDef();
    ret->setIdentifier(createTypeIdentifier(name));
    ret->setAttributes(attrs);
    return ret;
}
InitializerDef* NodeFactory::createInitializer(const std::vector<Attribute*>& attrs)
{
    InitializerDef* ret = new InitializerDef();
    ret->setAttributes(attrs);
    return ret;
}
DeinitializerDef* NodeFactory::createDeinitializer(const std::vector<Attribute*>& attrs)
{
    DeinitializerDef* ret = new DeinitializerDef();
    ret->setAttributes(attrs);
    return ret;
}
ExtensionDef* NodeFactory::createExtension(const std::vector<Attribute*>& attrs)
{
    ExtensionDef* ret = new ExtensionDef();
    ret->setAttributes(attrs);
    return ret;
}
SubscriptDef* NodeFactory::createSubscript(const std::vector<Attribute*>& attrs)
{
    SubscriptDef* ret = new SubscriptDef();
    ret->setAttributes(attrs);
    return ret;
}
OperatorDef* NodeFactory::createOperator(const std::vector<Attribute*>& attrs)
{
    OperatorDef* ret = new OperatorDef();
    ret->setAttributes(attrs);
    return ret;
}
GenericConstraint* NodeFactory::createGenericConstraint()
{
    return new GenericConstraint();
}
GenericParameters* NodeFactory::createGenericParameters()
{
    return new GenericParameters();
}



