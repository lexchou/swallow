#include "SemanticAnalyzer.h"
#include "GlobalScope.h"
#include "SymbolRegistry.h"
#include "ast/ast.h"
#include "swift_errors.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "ast/NodeSerializer.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "TypeBuilder.h"
#include "FunctionIterator.h"
#include <cassert>
#include <algorithm>


USE_SWIFT_NS
using namespace std;
bool SemanticAnalyzer::isInteger(const TypePtr& type)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    for(const TypePtr& t : scope->t_Ints)
    {
        if(t == type)
            return true;
    }
    return false;
}
bool SemanticAnalyzer::isNumber(const TypePtr& type)
{
    if(isFloat(type))
        return true;
    if(isInteger(type))
        return true;
    return false;
}
bool SemanticAnalyzer::isFloat(const TypePtr& type)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(type == scope->t_Float || type == scope->t_Double)
        return true;
    return false;
}

void SemanticAnalyzer::checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer)
{
    //this is a tuple definition, the corresponding declared type must be a tuple type
    TypeNodePtr declaredType = tuple->getDeclaredType();
    TypePtr type = lookupType(declaredType);
    if(!type)
    {
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_USE_OF_UNDECLARED_TYPE_1, out.str());
        return;
    }
    if(!(type->getCategory() == Type::Tuple))
    {
        //tuple definition must have a tuple type definition
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, out.str());
        return;
    }
    if(tuple->numElements() != type->numElementTypes())
    {
        //tuple pattern has the wrong length for tuple type '%'
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, out.str());
        return;
    }
    //check if initializer has the same type with the declared type
    if(initializer)
    {
        TypePtr valueType = evaluateType(initializer);
        if(valueType && *valueType != *type)
        {
            //tuple pattern has the wrong length for tuple type '%'
            std::wstringstream out;
            NodeSerializerW serializer(out);
            declaredType->accept(&serializer);
            //tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)
            wstring expectedType = type->toString();
            wstring got = toString(valueType->numElementTypes());
            wstring expected = toString(type->numElementTypes());
            error(initializer, Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4, out.str(), expectedType, got, expected);
            return;
        }
    }


    for(const Tuple::Element& p : *tuple)
    {
        NodeType::T nodeType = p.element->getNodeType();
        if(nodeType != NodeType::Identifier)
        {

        }

    }
}


TypePtr SemanticAnalyzer::getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score)
{
    if(expr->getType() == nullptr)
        expr->accept(this);
    score = 1;
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(expr->getNodeType() == NodeType::IntegerLiteral && hint != nullptr)
    {
        IntegerLiteralPtr literal = std::static_pointer_cast<IntegerLiteral>(expr);
        if(isFloat(hint))
        {
            score  = 0.5;
            return hint;
        }
        if(isInteger(hint))
            return hint;
        for(const TypePtr& t : scope->t_Ints)
        {
            if(t == hint)
                return t;
        }
    }
    if(expr->getNodeType() == NodeType::FloatLiteral && hint != nullptr)
    {
        FloatLiteralPtr literal = std::static_pointer_cast<FloatLiteral>(expr);
        if(isFloat(hint))
            return hint;
    }
    TypePtr ret = expr->getType();
    assert(ret != nullptr);
    return ret;
}
/**
*
* @param parameter
* @param argument
* @param variadic  Variadic parameter must have no name
* @param score
* @param supressErrors
*/
bool SemanticAnalyzer::checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const ParenthesizedExpression::Term& argument, bool variadic, float& score, bool supressErrors)
{

    const std::wstring name = argument.first;
    float s = 1;
    TypePtr argType = getExpressionType(argument.second, parameter.type, s);

    if(variadic)
    {
        //variadic parameter must have no name
        if(!name.empty())
        {
            error(argument.second, Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, name);
            abort();
        }
    }
    else
    {
        if (parameter.name != name)
        {
            if (!supressErrors)
            {
                if (name.empty() && !parameter.name.empty())
                {
                    error(argument.second, Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1, parameter.name);
                }
                else
                {
                    error(argument.second, Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, name);
                }
                abort();
            }
            return false;
        }
    }
    //only consider it's a generic parameter when and only when the generic definition is provided by the function
    //otherwise the generic parameter may be provided by the type definition
    if((parameter.type->getCategory() == Type::GenericParameter) && (funcType->getGenericDefinition() != nullptr))
    {
        //check for type constraint
        GenericDefinitionPtr generic = funcType->getGenericDefinition();
        assert(generic != nullptr);
        TypePtr expectedType;
        bool constraintSatisfied = generic->validate(parameter.type->getName(), argType, expectedType);
        if(!constraintSatisfied)
        {
            if(supressErrors)
                return false;
            error(argument.second, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, argType->getName(), expectedType->getName());
            abort();
        }
    }
    else if(*argType != *parameter.type)
    {
        if (!supressErrors)
        {
            error(argument.second, Errors::E_UNMATCHED_PARAMETERS);
            abort();
        }
        return false;//parameter is not matched
    }
    score += s;
    return true;
}

float SemanticAnalyzer::calculateFitScore(const TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors)
{
    float score = 0;
    const std::vector<Type::Parameter>& parameters = func->getParameters();
    bool variadic = func->hasVariadicParameters();

    //TODO: check trailing closure


    std::vector<Type::Parameter>::const_iterator paramIter = parameters.begin();
    std::vector<ParenthesizedExpression::Term>::const_iterator argumentIter = arguments->begin();
    std::vector<Type::Parameter>::const_iterator paramEnd = variadic ? parameters.end() - 1 : parameters.end();
    for(;argumentIter != arguments->end() && paramIter != paramEnd; argumentIter++, paramIter++)
    {
        const Type::Parameter& parameter = *paramIter;
        const ParenthesizedExpression::Term& argument = *argumentIter;
        bool ret = checkArgument(func, parameter, argument, false, score, supressErrors);
        if(!ret)
            return -1;
    }
    //if there's the rest parameters, they must have initializer
    if(paramIter != paramEnd && argumentIter == arguments->end())
    {
        //if there's only one parameter and it's a variadic parameter, ignore this error
        if(!(paramIter + 1 == parameters.end() && variadic))
        {
            //TODO: check initializer
            if (!supressErrors)
            {
                error(arguments, Errors::E_UNMATCHED_PARAMETERS);
                abort();
            }
            return -1;
        }
    }
    //if there's the rest arguments, check for variadic
    if(paramIter == paramEnd && argumentIter != arguments->end())
    {
        if(!variadic)
        {
            if(!supressErrors)
            {
                error(argumentIter->second, Errors::E_EXTRANEOUS_ARGUMENT);
                abort();
            }
            return -1;
        }
        const Type::Parameter& parameter = *paramIter;
        //the first variadic argument must have a label if the parameter got a label
        if(!parameter.name.empty())
        {
            bool ret = checkArgument(func, parameter, *argumentIter++, false, score, supressErrors);
            if(!ret)
                return -1;
        }

        //check rest argument
        for(;argumentIter != arguments->end(); argumentIter++)
        {
            bool ret = checkArgument(func, parameter, *argumentIter, true, score, supressErrors);
            if(!ret)
                return -1;
        }
    }
    if(!arguments->numExpressions())
        return 1;
    return score / arguments->numExpressions();
}

void SemanticAnalyzer::visitFunctionCall(const IdentifierPtr& name, const FunctionCallPtr& node)
{
    //visitFunctionCall(sym, node);
}
FunctionSymbolPtr SemanticAnalyzer::getOverloadedFunction(const NodePtr& node, const FunctionOverloadedSymbolPtr& funcs, const ParenthesizedExpressionPtr& arguments)
{
    typedef std::pair<float, FunctionSymbolPtr> ScoredFunction;
    std::vector<ScoredFunction> candidates;
    for(FunctionSymbolPtr func : *funcs)
    {
        float score = calculateFitScore(func->getType(), arguments, true);
        if(score > 0)
            candidates.push_back(std::make_pair(score, func));
    }
    if(candidates.empty())
    {
        error(node, Errors::E_NO_MATCHED_OVERLOAD);
        abort();
    }
    //sort by fit score
    if(candidates.size() > 1)
    {
        sort(candidates.begin(), candidates.end(), [](const ScoredFunction& lhs, const ScoredFunction& rhs ){
            return rhs.first < lhs.first;
        });
        if(candidates[0].first == candidates[1].first)
        {
            error(node, Errors::E_AMBIGUOUS_USE_1, funcs->getName());
            abort();
        }
    }
    FunctionSymbolPtr matched = candidates.front().second;
    return matched;
}
void SemanticAnalyzer::visitFunctionCall(const SymbolPtr& sym, const FunctionCallPtr& node)
{
    //Prepare the arguments
    for(const ParenthesizedExpression::Term& term : *node->getArguments())
    {
        term.second->accept(this);
    }

    if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        //verify argument
        std::wstring name = func->getName();
        calculateFitScore(func->getType(), node->getArguments(), false);
        node->setType(func->getReturnType());
    }
    else if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        if(funcs->numOverloads() == 1)
        {
            FunctionSymbolPtr func = *funcs->begin();
            //verify argument
            std::wstring name = func->getName();
            calculateFitScore(func->getType(), node->getArguments(), false);
            node->setType(func->getReturnType());
        }
        else
        {
            FunctionSymbolPtr matched = getOverloadedFunction(node, funcs, node->getArguments());
            node->setType(matched->getReturnType());
        }
    }
    else
    {
        assert(0 && "Unsupported function to call");
    }

}

void SemanticAnalyzer::visitReturn(const ReturnStatementPtr& node)
{
    SemanticNodeVisitor::visitReturn(node);
    Node* owner = symbolRegistry->getCurrentScope()->getOwner();
    assert(owner != nullptr);
    TypePtr funcType;
    switch(owner->getNodeType())
    {
        case NodeType::Program:
            return;//return type checking is ignored in program scope
        case NodeType::CodeBlock:
            funcType = static_cast<CodeBlock*>(owner)->getType();
            assert(funcType != nullptr);
            break;
        case NodeType::Closure:
            funcType = static_cast<Closure*>(owner)->getType();
            assert(funcType != nullptr);
            break;
        default:
            assert(0 && "Unsupported scope of return keyword detected.");
            break;

    }
    float score = 0;
    TypePtr retType = this->getExpressionType(node->getExpression(), funcType->getReturnType(), score);
    TypePtr expectedType = funcType->getReturnType();
    if(!Type::equals(retType, expectedType))
    {
        error(node->getExpression(), Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, retType->toString(), expectedType->toString());
    }
}
void SemanticAnalyzer::visitFunctionCall(const FunctionCallPtr& node)
{
    NodeType::T nodeType = node->getFunction()->getNodeType();
    ExpressionPtr func = node->getFunction();
    func->accept(this);

    switch(nodeType)
    {
        case NodeType::Identifier:
        {
            IdentifierPtr id = std::static_pointer_cast<Identifier>(func);
            const std::wstring &symbolName = id->getIdentifier();
            SymbolPtr sym = symbolRegistry->lookupSymbol(symbolName);
            if (!sym)
            {
                error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, symbolName);
                return;
            }
            //if symbol points to a type, then redirect it to a initializer
            if (TypePtr type = std::dynamic_pointer_cast<Type>(sym))
            {
                if (type->getCategory() == Type::Class || type->getCategory() == Type::Struct)
                {
                    sym = type->getInitializer();
                }
            }
            visitFunctionCall(sym, node);
            break;
        }
        case NodeType::MemberAccess:
        {
            MemberAccessPtr ma = std::static_pointer_cast<MemberAccess>(func);
            TypePtr selfType = ma->getSelf()->getType();
            assert(selfType != nullptr);
            SymbolPtr sym = selfType->getMember(ma->getField()->getIdentifier());
            assert(sym != nullptr);//
            visitFunctionCall(sym, node);
            break;
        }
        case NodeType::Closure:
        {
            ClosurePtr closure = std::static_pointer_cast<Closure>(func);
            TypePtr type = closure->getType();
            assert(type != nullptr && type->getCategory() == Type::Function);
            calculateFitScore(type, node->getArguments(), false);
            node->setType(type->getReturnType());
            break;
        }
        case NodeType::ArrayLiteral:
        {
            //check if it's an array expression
            ArrayLiteralPtr array = std::static_pointer_cast<ArrayLiteral>(func);
            if(array->numElements() != 1 || (*array->begin())->getNodeType() != NodeType::Identifier)
            {
                wstring call = toString(func);
                wstring type = func->getType()->toString();
                error(func, Errors::E_INVALID_CALL_OF_NON_FUNCTION_TYPE_2, call, type);
                break;
            }
            IdentifierPtr typeName = std::static_pointer_cast<Identifier>(*array->begin());
            SymbolPtr sym = symbolRegistry->lookupSymbol(typeName->getIdentifier());
            TypePtr type = std::dynamic_pointer_cast<Type>(sym);
            if(!type)
            {
                error(func, Errors::E_INVALID_CALL_OF_NON_FUNCTION_TYPE_2, toString(func), func->getType()->toString());
                break;
            }
            TypePtr Array = symbolRegistry->getGlobalScope()->t_Array;
            TypePtr arrayType = Type::newSpecializedType(Array, type);
            node->setType(arrayType);
            break;
        }
        case NodeType::DictionaryLiteral:
        {
            break;
        }
        default:
        {
            error(func, Errors::E_INVALID_CALL_OF_NON_FUNCTION_TYPE_2, toString(func), func->getType()->toString());
            break;
        }
    }

}
void SemanticAnalyzer::visitMemberAccess(const MemberAccessPtr& node)
{
    node->getSelf()->accept(this);
    TypePtr selfType = node->getSelf()->getType();
    assert(selfType != nullptr);


    SymbolPtr member = selfType->getMember(node->getField()->getIdentifier());
    if(member == nullptr)
    {
        error(node->getField(), Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), node->getField()->getIdentifier());
        return;
    }
    node->setType(member->getType());
}

void SemanticAnalyzer::visitString(const StringLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    node->setType(scope->t_String);
}
void SemanticAnalyzer::visitInteger(const IntegerLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    node->setType(scope->t_Int);
}
void SemanticAnalyzer::visitFloat(const FloatLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    node->setType(scope->t_Double);
}

bool SemanticAnalyzer::canConvertTo(const ExpressionPtr& expr, const TypePtr& type)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return isNumber(type);
        case NodeType::FloatLiteral:
            return isFloat(type);
        default:
            return false;
    }
    return false;
}

void SemanticAnalyzer::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    int num = node->numElements();
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint)
    {
        //TODO if hint specified, it must be Array type nor conform to ArrayLiteralConvertible protocol
        if(t_hint->getInnerType() != scope->t_Array)
        {
            bool conformToArrayLiteralConvertible = false;
            if(!conformToArrayLiteralConvertible)
            {
                error(node, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, t_hint->getName(), L"ArrayLiteralConvertible");
                return;
            }
        }
    }



    if(num == 0)
    {
        if(!t_hint)//cannot define an empty array without type hint.
            error(node, Errors::E_CANNOT_DEFINE_EMPTY_ARRAY_WITHOUT_TYPE);
        return;
    }
    bool hasHint = t_hint != nullptr;
    TypePtr type = t_hint != nullptr ? t_hint->getInnerType() : nullptr;
    TypePtr hint;
    for(const ExpressionPtr& el : *node)
    {
        el->accept(this);
        if(type != nullptr)
        {
            //check if element can be converted into given type
            if(!canConvertTo(el, type))
            {
                error(el, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(el), type->getName());
            }
        }
        else
        {
            switch(el->getNodeType())
            {
                case NodeType::IntegerLiteral:
                    hint = scope->t_Int;
                    break;
                case NodeType::FloatLiteral:
                    hint = scope->t_Double;
                    break;
                default:
                    type = el->getType();
                    if(hint)
                    {
                        //check if the hint can be converted to type
                        bool success = (hint == scope->t_Double && isFloat(type)) || (hint == scope->t_Int && isNumber(type));
                        if(!success)
                        {
                            error(el, Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES);
                        }

                    }
                    break;
            }
        }
    }

    assert(type != nullptr || hint != nullptr);

    if(!type && hint)
        type = hint;

    TypePtr arrayType = Type::newSpecializedType(scope->t_Array, type);
    node->setType(arrayType);
}
void SemanticAnalyzer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{

}
void SemanticAnalyzer::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    SemanticNodeVisitor::visitParenthesizedExpression(node);
    std::vector<TypePtr> types;
    for(const ParenthesizedExpression::Term& element : *node)
    {
        TypePtr elementType = element.second->getType();
        assert(elementType != nullptr);
        types.push_back(elementType);
    }
    TypePtr type = Type::newTuple(types);
    node->setType(type);
}
void SemanticAnalyzer::visitTuple(const TuplePtr& node)
{
    SemanticNodeVisitor::visitTuple(node);
    std::vector<TypePtr> types;
    for(const Tuple::Element& element : *node)
    {
        TypePtr elementType = element.element->getType();
        assert(elementType != nullptr);
        types.push_back(elementType);
    }
    TypePtr type = Type::newTuple(types);
    node->setType(type);
}

void SemanticAnalyzer::visitOperator(const OperatorDefPtr& node)
{
    //register operator
    if(node->getType() == OperatorType::InfixBinary)
    {
        //check precedence range
        if(node->getPrecedence() < 0 || node->getPrecedence() > 255)
        {
            error(node, Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE);
            abort();
        }
    }

    bool r = symbolRegistry->registerOperator(node->getName(), node->getType(), node->getAssociativity(), node->getPrecedence());
    if(!r)
    {
        error(node, Errors::E_OPERATOR_REDECLARED);
        abort();
    }
}
void SemanticAnalyzer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}
void SemanticAnalyzer::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    SemanticNodeVisitor::visitBinaryOperator(node);
    //look for binary function that matches
    OperatorInfo* op = symbolRegistry->getOperator(node->getOperator());
    SymbolPtr sym = symbolRegistry->lookupSymbol(node->getOperator());
    if(!op || !sym)
    {
        error(node, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, node->getOperator());
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        abort();
    }
    if((op->type & OperatorType::InfixBinary) == 0)
    {
        error(node, Errors::E_IS_NOT_BINARY_OPERATOR_1, node->getOperator());
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        abort();
    }
    //find for overload
    FunctionSymbolPtr func = nullptr;
    FunctionOverloadedSymbolPtr overloaded = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym);
    if(overloaded)
    {
        TypePtr argv[2];
        argv[0] = node->getLHS()->getType();
        argv[1] = node->getRHS()->getType();
        func = overloaded->lookupOverload(2, argv);
    }
    else
    {
        func = std::dynamic_pointer_cast<FunctionSymbol>(sym);
    }
    if(!func)
    {
        error(node, Errors::E_NO_OVERLOAD_ACCEPTS_ARGUMENTS_1, node->getOperator());
        abort();
    }
    node->setType(func->getReturnType());
}
void SemanticAnalyzer::visitUnaryOperator(const UnaryOperatorPtr& node)
{

}


void SemanticAnalyzer::visitCompileConstant(const CompileConstantPtr& node)
{
    const std::wstring& name = node->getName();
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(name == L"__LINE__" || name == L"__COLUMN__")
    {
        node->setType(scope->t_Int);
    }
    else if(name == L"__FUNCTION__" || name == L"__FILE__")
    {
        node->setType(scope->t_String);
    }
    else
    {
        assert(0 && "Invalid compile constant");
        abort();
    }
}




TypePtr SemanticAnalyzer::evaluateType(const ExpressionPtr& expr)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return symbolRegistry->lookupType(L"Int");
        case NodeType::FloatLiteral:
            return symbolRegistry->lookupType(L"Float");
        case NodeType::StringLiteral:
            return symbolRegistry->lookupType(L"String");
        default:
            return nullptr;
    }
}
void SemanticAnalyzer::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    node->getSelf()->accept(this);
    TypePtr selfType = node->getSelf()->getType();
    assert(selfType != nullptr);
    FunctionOverloadedSymbolPtr funcs = selfType->getSubscript();
    if(funcs == nullptr)
    {
        //undefined subscript access
        wstring s = this->toString(node);
        error(node, Errors::E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1, s);
        abort();
    }
    //Now inference the type returned by this subscript access
    FunctionSymbolPtr func = getOverloadedFunction(node, funcs, node->getIndex());
    node->setType(func->getReturnType());
}
