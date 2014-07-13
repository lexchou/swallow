#include "node-factory.h"
#include "ast/ast.h"
#include "tokenizer/tokens.h"
USE_SWIFT_NS;


NodeFactory::NodeFactory()
    :autoReleasePool(NULL)
{
}
void NodeFactory::setAutoReleasePool(AutoReleasePool* autoReleasePool)
{
    this->autoReleasePool = autoReleasePool;
}

void NodeFactory::bindNode(const SourceInfo&s, Node* n)
{
    *n->getSourceInfo() = s;
    n->setAutoReleasePool(autoReleasePool);
}


CommentNode* NodeFactory::createComment(const SourceInfo& state, const std::wstring& comment)
{
    return _(state, new CommentNode(comment));
}
BooleanLiteral* NodeFactory::createBoolean(const SourceInfo& state, bool value)
{
    return _(state, new BooleanLiteral(value));
}
IntegerLiteral* NodeFactory::createInteger(const SourceInfo& state, const std::wstring& value)
{
    return _(state, new IntegerLiteral(value));
}
FloatLiteral* NodeFactory::createFloat(const SourceInfo& state, const std::wstring& value)
{
    return _(state, new FloatLiteral(value));
}
StringLiteral* NodeFactory::createString(const SourceInfo& state, const std::wstring& value)
{
    return _(state, new StringLiteral(value));
}
UnaryOperator* NodeFactory::createUnary(const SourceInfo& state, const std::wstring& op, OperatorType::T type)
{
    return _(state, new UnaryOperator(op, type));
}
Identifier* NodeFactory::createIdentifier(const SourceInfo& state, const std::wstring& op)
{
    return _(state, new Identifier(op));
}
GenericArgument* NodeFactory::createGenericArgument(const SourceInfo& state)
{
    return _(state, new GenericArgument());
}
InOutParameter* NodeFactory::createInOutParameter(const SourceInfo& state, Identifier* identifier)
{
    InOutParameter* ret = _(state, new InOutParameter(identifier));
    return ret;
}
BinaryOperator* NodeFactory::createBinary(const SourceInfo& state, const std::wstring& op, Associativity::T associativity, int precedence)
{
    BinaryOperator* ret = _(state, new BinaryOperator(op, associativity, precedence));
    return ret;
}

ArrayLiteral* NodeFactory::createArrayLiteral(const SourceInfo& state)
{
    return _(state, new ArrayLiteral());
}
DictionaryLiteral* NodeFactory::createDictionaryLiteral(const SourceInfo& state)
{
    return _(state, new DictionaryLiteral());
}
CompileConstant* NodeFactory::createCompilecConstant(const SourceInfo& state, const std::wstring& name, const std::wstring& value)
{
    return _(state, new CompileConstant(name, value));
}

MemberAccess* NodeFactory::createMemberAccess(const SourceInfo& state, Expression* self, Identifier* field)
{
    return _(state, new MemberAccess(self, field));
}
SubscriptAccess* NodeFactory::createSubscriptAccess(const SourceInfo& state, Expression* self, Expression* index)
{
    return _(state, new SubscriptAccess(self, index));
}

TypeCheck* NodeFactory::createTypeCheck(const SourceInfo& state, Pattern* expr, TypeNode* type)
{
    return _(state, new TypeCheck(expr, type));
}
TypeCast* NodeFactory::createTypeCast(const SourceInfo& state, Expression* expr, TypeNode* type)
{
    return _(state, new TypeCast(expr, type));
}
Assignment* NodeFactory::createAssignment(const SourceInfo& state, Expression* lhs, Expression* rhs)
{
    return _(state, new Assignment(lhs, rhs));
}
ConditionalOperator* NodeFactory::createConditionalOperator(const SourceInfo& state, Expression* cond, Expression* trueExpr, Expression* falseExpr)
{
    return _(state, new ConditionalOperator(cond, trueExpr, falseExpr));
}
ParenthesizedExpression* NodeFactory::createParenthesizedExpression(const SourceInfo& state)
{
    return _(state, new ParenthesizedExpression());
}

InitializerReference* NodeFactory::createInitializerReference(const SourceInfo& state, Expression* expr)
{
    return _(state, new InitializerReference(expr));
}
SelfExpression* NodeFactory::createSelfExpression(const SourceInfo& state, Expression* expr)
{
    return _(state, new SelfExpression(expr));
}
DynamicType* NodeFactory::createDynamicType(const SourceInfo& state, Expression* expr)
{
    return _(state, new DynamicType(expr));
}
ForcedValue* NodeFactory::createForcedValue(const SourceInfo& state, Expression* expr)
{
    return _(state, new ForcedValue(expr));
}
OptionalChaining* NodeFactory::createOptionalChaining(const SourceInfo& state, Expression* expr)
{
    return _(state, new OptionalChaining(expr));
}
FunctionCall* NodeFactory::createFunctionCall(const SourceInfo& state)
{
    return _(state, new FunctionCall());
}


ForLoop* NodeFactory::createForLoop(const SourceInfo& state)
{
    return _(state, new ForLoop());
}
ForInLoop* NodeFactory::createForInLoop(const SourceInfo& state)
{
    return _(state, new ForInLoop());
}
WhileLoop* NodeFactory::createWhileLoop(const SourceInfo& state)
{
    return _(state, new WhileLoop());
}
IfStatement* NodeFactory::createIf(const SourceInfo& state)
{
    return _(state, new IfStatement());
}
DoLoop* NodeFactory::createDoLoop(const SourceInfo& state)
{
    return _(state, new DoLoop());
}
SwitchCase* NodeFactory::createSwitch(const SourceInfo& state)
{
    return _(state, new SwitchCase());
}
CaseStatement* NodeFactory::createCase(const SourceInfo& state)
{
    return _(state, new CaseStatement());
}
BreakStatement* NodeFactory::createBreak(const SourceInfo& state)
{
    return _(state, new BreakStatement());
}
ContinueStatement* NodeFactory::createContinue(const SourceInfo& state)
{
    return _(state, new ContinueStatement());
}
FallthroughStatement* NodeFactory::createFallthrough(const SourceInfo& state)
{
    return _(state, new FallthroughStatement());
}
ReturnStatement* NodeFactory::createReturn(const SourceInfo& state)
{
    return _(state, new ReturnStatement());
}
LabeledStatement* NodeFactory::createLabel(const SourceInfo& state)
{
    return _(state, new LabeledStatement());
}
CodeBlock* NodeFactory::createCodeBlock(const SourceInfo& state)
{
    return _(state, new CodeBlock());
}
LetBinding* NodeFactory::createLetBinding(const SourceInfo& state)
{
    return _(state, new LetBinding());
}
VarBinding* NodeFactory::createVarBinding(const SourceInfo& state)
{
    return _(state, new VarBinding());
}
Tuple* NodeFactory::createTuple(const SourceInfo& state)
{
    return _(state, new Tuple());
}
Closure* NodeFactory::createClosure(const SourceInfo& state)
{
    return _(state, new Closure());
}
EnumCasePattern* NodeFactory::createEnumCasePattern(const SourceInfo& state, const std::wstring& name)
{
    EnumCasePattern* ret = _(state, new EnumCasePattern());
    ret->setName(name);
    return ret;
}


FunctionType* NodeFactory::createFunctionType(const SourceInfo& state, TypeNode* argumentsType, TypeNode* retType)
{
    FunctionType* ret = _(state, new FunctionType());
    ret->setArgumentsType(argumentsType);
    ret->setReturnType(retType);
    return ret;
}
ArrayType* NodeFactory::createArrayType(const SourceInfo& state, TypeNode* innerType)
{
    ArrayType* ret = _(state, new ArrayType());
    ret->setInnerType(innerType);
    return ret;
}
OptionalType* NodeFactory::createOptionalType(const SourceInfo& state, TypeNode* innerType)
{
    OptionalType* ret = _(state, new OptionalType());
    ret->setInnerType(innerType);
    return ret;
}
ImplicitlyUnwrappedOptional* NodeFactory::createImplicitlyUnwrappedOptional(const SourceInfo& state, TypeNode* innerType)
{
    ImplicitlyUnwrappedOptional* ret = _(state, new ImplicitlyUnwrappedOptional());
    ret->setInnerType(innerType);
    return ret;
}
TypeIdentifier* NodeFactory::createTypeIdentifier(const SourceInfo& state, const std::wstring& typeName)
{
    TypeIdentifier* ret = _(state, new TypeIdentifier());
    ret->setName(typeName);
    return ret;
}
ProtocolComposition* NodeFactory::createProtocolComposition(const SourceInfo& state)
{
    return _(state, new ProtocolComposition());
}
TupleType* NodeFactory::createTupleType(const SourceInfo& state)
{
    return _(state, new TupleType());
}
Attribute* NodeFactory::createAttribute(const SourceInfo& state)
{
    Attribute* ret = _(state, new Attribute());
    return ret;
}



Import* NodeFactory::createImport(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    Import* ret = _(state, new Import());
    ret->setAttributes(attrs);
    return ret;
}
Constant* NodeFactory::createConstant(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers)
{
    Constant* ret = _(state, new Constant());
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}
Variables* NodeFactory::createVariables(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers)
{
    Variables* ret = _(state, new Variables());
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}

Variable* NodeFactory::createVariable(const SourceInfo& state)
{
    Variable* ret = _(state, new Variable());
    return ret;
}
TypeAlias* NodeFactory::createTypealias(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    TypeAlias* ret = _(state, new TypeAlias());
    ret->setAttributes(attrs);
    return ret;
}
FunctionDef* NodeFactory::createFunction(const SourceInfo& state, const std::vector<Attribute*>& attrs, int specifiers)
{
    FunctionDef* ret = _(state, new FunctionDef());
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    return ret;
}
Parameters* NodeFactory::createParameters(const SourceInfo& state)
{
    return _(state, new Parameters());
}
Parameter* NodeFactory::createParameter(const SourceInfo& state)
{
    return _(state, new Parameter());
}
EnumDef* NodeFactory::createEnum(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    EnumDef* ret = _(state, new EnumDef());
    ret->setAttributes(attrs);
    return ret;
}
StructDef* NodeFactory::createStruct(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    StructDef* ret = _(state, new StructDef());
    ret->setAttributes(attrs);
    return ret;
}
ClassDef* NodeFactory::createClass(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ClassDef* ret = _(state, new ClassDef());
    ret->setAttributes(attrs);
    return ret;
}
ProtocolDef* NodeFactory::createProtocol(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ProtocolDef* ret = _(state, new ProtocolDef());
    ret->setAttributes(attrs);
    return ret;
}
InitializerDef* NodeFactory::createInitializer(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    InitializerDef* ret = _(state, new InitializerDef());
    ret->setAttributes(attrs);
    return ret;
}
DeinitializerDef* NodeFactory::createDeinitializer(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    DeinitializerDef* ret = _(state, new DeinitializerDef());
    ret->setAttributes(attrs);
    return ret;
}
ExtensionDef* NodeFactory::createExtension(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    ExtensionDef* ret = _(state, new ExtensionDef());
    ret->setAttributes(attrs);
    return ret;
}
SubscriptDef* NodeFactory::createSubscript(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    SubscriptDef* ret = _(state, new SubscriptDef());
    ret->setAttributes(attrs);
    return ret;
}
OperatorDef* NodeFactory::createOperator(const SourceInfo& state, const std::vector<Attribute*>& attrs)
{
    OperatorDef* ret = _(state, new OperatorDef());
    ret->setAttributes(attrs);
    return ret;
}
GenericConstraint* NodeFactory::createGenericConstraint(const SourceInfo& state)
{
    return _(state, new GenericConstraint());
}
GenericParameters* NodeFactory::createGenericParameters(const SourceInfo& state)
{
    return _(state, new GenericParameters());
}



