#ifndef TYPE_INFERENCE_ACTION_H
#define TYPE_INFERENCE_ACTION_H
#include "swift_conf.h"
#include "ast/NodeVisitor.h"
#include "SemanticNodeVisitor.h"
#include "type.h"
#include "ast/ParenthesizedExpression.h"
SWIFT_NS_BEGIN


class Type;
class Expression;
class TypeInferenceAction : public SemanticNodeVisitor
{
public:
    TypeInferenceAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& node) override;
    virtual void visitCompileConstant(const CompileConstantPtr& node) override;
    virtual void visitMemberAccess(const MemberAccessPtr& node) override;
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitReturn(const ReturnStatementPtr& node) override;
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node) override;
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node) override;
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitSubscript(const SubscriptDefPtr& node) override;
    virtual void visitSubscriptAccess(const SubscriptAccessPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
private:
    void visitFunctionCall(const IdentifierPtr& name, const FunctionCallPtr& node);
    void visitFunctionCall(const SymbolPtr& func, const FunctionCallPtr& node);
private:
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    TypePtr evaluateType(const ExpressionPtr& expr);

    /**
     * Calculates the fit score of arguments on given function
     * @return -1 if the type is not matched
     */
    float calculateFitScore(const TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors);
    bool checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const ParenthesizedExpression::Term& argument, bool variadic, float& score, bool supressErrors);
    TypePtr getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score);

    /**
     * Return a function that matches the given argument
     * This will always returns a matched function, if no functions matched it will throw exception and abort the process
     */
    FunctionSymbolPtr getOverloadedFunction(const NodePtr& node, const FunctionOverloadedSymbolPtr& funcs, const ParenthesizedExpressionPtr& arguments);
    /**
     * Check if the given expression can be converted to given type
     */
    bool canConvertTo(const ExpressionPtr&, const TypePtr& type);

    bool isInteger(const TypePtr& type);
    bool isNumber(const TypePtr& type);
    bool isFloat(const TypePtr& type);
private:
    //hint for parsing Array/tuple/dictionary literal
    TypePtr t_hint;

};


SWIFT_NS_END


#endif//TYPE_INFERENCE_ACTION_H
