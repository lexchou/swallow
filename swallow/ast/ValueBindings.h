#ifndef VALUE_BINDINGS_H
#define VALUE_BINDINGS_H
#include "Declaration.h"
#include <string>

SWIFT_NS_BEGIN

    typedef std::shared_ptr<class ValueBinding> ValueBindingPtr;

    class ValueBindings : public Declaration
    {
    public:
        ValueBindings();
        ~ValueBindings();
    public:
        using Declaration::setAttributes;
        using Declaration::setSpecifiers;
        using Declaration::getAttributes;
        using Declaration::getSpecifiers;
    public:
        void add(const ValueBindingPtr& var);
        ValueBindingPtr get(int i);
        int numBindings();

        bool isReadOnly()const { return readOnly;}
        void setReadOnly(bool readOnly) { this->readOnly = readOnly;}

        std::vector<ValueBindingPtr>::iterator begin() {return valueBindings.begin();}
        std::vector<ValueBindingPtr>::iterator end() {return valueBindings.end();}
    public://Node
        virtual void accept(NodeVisitor* visitor);
    public:
        std::vector<ValueBindingPtr> valueBindings;
        bool readOnly;
    };

SWIFT_NS_END

#endif//VALUE_BINDINGS_H