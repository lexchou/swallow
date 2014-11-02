#ifndef VALUE_BINDINGS_H
#define VALUE_BINDINGS_H
#include "Declaration.h"
#include <string>
#include <list>

SWIFT_NS_BEGIN

    typedef std::shared_ptr<class ValueBinding> ValueBindingPtr;

    class ValueBindings : public Declaration
    {
    public:
        typedef std::list<ValueBindingPtr>::iterator Iterator;
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

        Iterator begin() {return valueBindings.begin();}
        Iterator end() {return valueBindings.end();}

        void insertAfter(const ValueBindingPtr& binding, const Iterator& iter);
    public://Node
        virtual void accept(NodeVisitor* visitor);
    public:
        std::list<ValueBindingPtr> valueBindings;
        bool readOnly;
    };

SWIFT_NS_END

#endif//VALUE_BINDINGS_H