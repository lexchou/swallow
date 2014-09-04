#ifndef TYPE_VERIFICATION_ACTION_H
#define TYPE_VERIFICATION_ACTION_H
#include "swift_conf.h"
#include "ast/NodeVisitor.h"
#include "SemanticNodeVisitor.h"
#include "type.h"
#include "ast/ParenthesizedExpression.h"
SWIFT_NS_BEGIN


    class Type;
    class Expression;

    /**
     * This action will verify type's protocol conformation.
     */
    class TypeVerificationAction : public SemanticNodeVisitor
    {
    public:
        TypeVerificationAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
    public:
        virtual void visitConstants(const ConstantsPtr& node) override;
        virtual void visitVariable(const VariablePtr& node) override;
        virtual void visitClass(const ClassDefPtr& node) override;
        virtual void visitStruct(const StructDefPtr& node) override;
        virtual void visitEnum(const EnumDefPtr& node) override;
        virtual void visitExtension(const ExtensionDefPtr& node) override;
        virtual void visitProtocol(const ProtocolDefPtr& node) override;
    private:
    private:
        /**
         * Verify if the specified type conform to the given protocol
         */
        void verifyProtocolConform(const TypePtr& type);
        void verifyProtocolConform(const TypePtr& type, const TypePtr& protocol);
        void verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected);
    private:
        TypePtr currentType;
    };


SWIFT_NS_END


#endif//TYPE_VERIFICATION_ACTION_H
