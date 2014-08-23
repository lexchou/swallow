#include "../utils.h"
#include "semantics/Type.h"


using namespace Swift;

TEST(TestType, testCovariance)
{
    TypePtr Base = Type::newType(L"Base", Type::Class);
    TypePtr Base2 = Type::newType(L"Base2", Type::Class, nullptr, Base);

    TypePtr Child1 = Type::newType(L"Child1", Type::Class, nullptr, Base2);
    TypePtr Child2 = Type::newType(L"Child2", Type::Class, nullptr, Base2);
    //calculate the common parent class between Child1 and Child2 with the minimum inheritance distance
    TypePtr parent = Child1->getCommonParent(Child2);
    ASSERT_NOT_NULL(parent);
    ASSERT_TRUE(parent == Base2);

}
