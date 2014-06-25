#include "utils.h"
#include "test-operator-expression.h"
#include "test-literal-expression.h"
#include "test-postfix-expression.h"
#include "test-statement.h"
using namespace Swift;

int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &alltest = CppUnit::TestFactoryRegistry::getRegistry("alltest");
    runner.addTest(alltest.makeTest());
    bool failed = !runner.run();
    return failed;
}
