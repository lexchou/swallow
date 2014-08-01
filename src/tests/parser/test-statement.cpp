#ifndef TEST_STATEMENT_H
#define TEST_STATEMENT_H

#include "tests/utils.h"

using namespace Swift;

class TestStatement : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestStatement);
    CPPUNIT_TEST(testFor);
    CPPUNIT_TEST(testFor2);
    CPPUNIT_TEST(testForIn);
    CPPUNIT_TEST(testWhile);
    CPPUNIT_TEST(testDo);
    CPPUNIT_TEST(testIf);
    CPPUNIT_TEST(testIf2);
    CPPUNIT_TEST(testIf3);
    CPPUNIT_TEST(testSwitch_Empty);
    CPPUNIT_TEST(testSwitch_MultipleCases);
    CPPUNIT_TEST(testSwitch_NoExplicitFallthrough);
    CPPUNIT_TEST(testSwitch_RangeMatching);
    CPPUNIT_TEST(testSwitch_Tuple);
    CPPUNIT_TEST(testSwitch_ValueBindings);
    CPPUNIT_TEST(testSwitch_Where);
    CPPUNIT_TEST(testSwitch_Enum);
    CPPUNIT_TEST(testSwitch_AssociatedValues);
    CPPUNIT_TEST(testLabeledWhile);
    CPPUNIT_TEST(testLabeledDo);
    CPPUNIT_TEST(testLabeledFor);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testFor()
    {
        PARSE_STATEMENT(L"for i = 0;i < 10; i++ {a = i * 2;println(a);}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoopPtr loop = std::dynamic_pointer_cast<ForLoop>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(1, loop->numInit());
        AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(loop->getInit(0));
        CPPUNIT_ASSERT(eq != NULL);
        
        BinaryOperatorPtr le = std::dynamic_pointer_cast<BinaryOperator>(loop->getCondition());
        ASSERT_EQUALS(L"<", le->getOperator().c_str());
        CPPUNIT_ASSERT(le != NULL);
        
        UnaryOperatorPtr step = std::dynamic_pointer_cast<UnaryOperator>(loop->getStep());
        CPPUNIT_ASSERT(step != NULL);
        ASSERT_EQUALS(L"++", step->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, step->getType());
        
        CodeBlockPtr body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(2, body->numStatements());
        CPPUNIT_ASSERT(eq = std::dynamic_pointer_cast<Assignment>(body->getStatement(0)));
        FunctionCallPtr call;
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(1)));
        IdentifierPtr func;
        CPPUNIT_ASSERT(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    
    void testFor2()
    {
        PARSE_STATEMENT(L"for ;; {}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoopPtr loop = std::dynamic_pointer_cast<ForLoop>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(0, loop->numInit());
        
        CPPUNIT_ASSERT(loop->getCondition() == NULL);
        CPPUNIT_ASSERT(loop->getStep() == NULL);
        
        
        CodeBlockPtr body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(0, body->numStatements());
        
        
    }
    void testForIn()
    {
        PARSE_STATEMENT(L"for index in 1...5 {\n"
                        L"println(\"\(index) times 5 is \(index * 5)\")\n"
                        L"}");
        ForInLoopPtr f;
        BinaryOperatorPtr op;
        IdentifierPtr id;
        
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<ForInLoop>(root));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(f->getLoopVars()));
        ASSERT_EQUALS(L"index", id->getIdentifier());
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(f->getContainer()));
        ASSERT_EQUALS(L"...", op->getOperator());
        
        
        
        
    }
    void testWhile()
    {
        PARSE_STATEMENT(L"while notTrue {println(a);}");
        CPPUNIT_ASSERT(root != NULL);
        WhileLoopPtr loop = std::dynamic_pointer_cast<WhileLoop>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        IdentifierPtr cond = std::dynamic_pointer_cast<Identifier>(loop->getCondition());
        CPPUNIT_ASSERT(cond != NULL);
        ASSERT_EQUALS(L"notTrue", cond->getIdentifier().c_str());
        
        
        CodeBlockPtr body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        FunctionCallPtr call;
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(0)));
        IdentifierPtr func;
        CPPUNIT_ASSERT(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    void testDo()
    {
        PARSE_STATEMENT(L"do{println(a);}while read(a)");
        CPPUNIT_ASSERT(root != NULL);
        DoLoopPtr loop = std::dynamic_pointer_cast<DoLoop>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(loop->getCondition());
        CPPUNIT_ASSERT(call != NULL);
        IdentifierPtr func = std::static_pointer_cast<Identifier>(call->getFunction());
        ASSERT_EQUALS(L"read", func->getIdentifier().c_str());
        
        
        CodeBlockPtr body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        CPPUNIT_ASSERT(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(0)));
        CPPUNIT_ASSERT(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    void testIf()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}");
        IfStatementPtr _if;
        BinaryOperatorPtr op;
        CPPUNIT_ASSERT(_if = std::dynamic_pointer_cast<IfStatement>(root));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatementPtr ret;
        CodeBlockPtr then;
        CPPUNIT_ASSERT(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
        IntegerLiteralPtr id;
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->valueAsString);
        CPPUNIT_ASSERT(_if->getElse() == NULL);
        
    }
    
    void testIf2()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}else{return 1;}");
        IfStatementPtr _if;
        BinaryOperatorPtr op;
        CPPUNIT_ASSERT(_if = std::dynamic_pointer_cast<IfStatement>(root));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatementPtr ret;
        CodeBlockPtr then, elsePart;
        
        CPPUNIT_ASSERT(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
        IntegerLiteralPtr id;
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->valueAsString);
        
        
        CPPUNIT_ASSERT(elsePart = std::dynamic_pointer_cast<CodeBlock>(_if->getElse()));
        CPPUNIT_ASSERT_EQUAL(1, elsePart->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(elsePart->getStatement(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"1", id->valueAsString);
        
    }
    
    void testIf3()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}else if a == 0 {return 0;} else {return 1}");
        
        
        IfStatementPtr _if;
        BinaryOperatorPtr op;
        CPPUNIT_ASSERT(_if = std::dynamic_pointer_cast<IfStatement>(root));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatementPtr ret;
        CodeBlockPtr then, elsePart;
        
        CPPUNIT_ASSERT(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
        IntegerLiteralPtr id;
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->valueAsString);
        
        
        CPPUNIT_ASSERT(_if = std::dynamic_pointer_cast<IfStatement>(_if->getElse()));
        
        CPPUNIT_ASSERT(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"0", id->valueAsString);
        
        
        CPPUNIT_ASSERT(elsePart = std::dynamic_pointer_cast<CodeBlock>(_if->getElse()));

        CPPUNIT_ASSERT_EQUAL(1, elsePart->numStatements());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(elsePart->getStatement(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
        ASSERT_EQUALS(L"1", id->valueAsString);
        
    }
    void testSwitch_Empty()
    {
        PARSE_STATEMENT(L"switch i{}");
        SwitchCasePtr sc;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(0, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() == NULL);

    }
    
    void testSwitch_MultipleCases()
    {
        PARSE_STATEMENT(L"switch someCharacter{"
                           L"case \"a\", \"e\", \"i\", \"o\", \"u\":"
                           L"print(\"vowel\")"
                           L"case \"b\", \"c\", \"d\":"
                           L"print(\"consonant\")"
                           L"default:"
                           L"print(\"other\")"
                           L"}");
        SwitchCasePtr sc;
        CaseStatementPtr c;
        StatementPtr st;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(5, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(st));
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(3, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(st));
        
    }
    
    void testSwitch_NoExplicitFallthrough()
    {
        PARSE_STATEMENT(L"switch anotherCharacter {"
                           L"case \"a\":"
                           L"case \"A\":"
                           L"println(\"The letter A\")"
                           L"default:"
                           L"println(\"Not the letter A\")"
                           L"}");
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        StatementPtr st;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(0, c->numStatements());

        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<FunctionCall>(st));
        
        
    }
    
    void testSwitch_RangeMatching()
    {
        PARSE_STATEMENT(L"switch count {"
                           L"case 0:"
                           L"naturalCount = \"no\""
                           L"case 1...3:"
                           L"naturalCount = \"a few\""
                           L"default:"
                           L"naturalCount = \"millions and millions of\""
                           L"}");
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        StatementPtr st;
        PatternPtr p;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(st));
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(p = c->getCondition(0).condition);
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Assignment>(st));
        
        
    }
    
    void testSwitch_Tuple()
    {
        
        PARSE_STATEMENT(L"switch somePoint {"
                           L"case (0, 0):"
                           L"println(\"(0, 0) is at the origin\")"
                           L"case (_, 0):"
                           L"println(\"(\(somePoint.0), 0) is on the x-axis\")"
                           L"}");
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        StatementPtr st;
        TuplePtr tuple;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(c->getCondition(0).condition));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(1)));
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(c->getCondition(0).condition));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(1)));
        
        
    }
    void testSwitch_ValueBindings()
    {
        PARSE_STATEMENT(L"switch anotherPoint {"
                           L"case (let x, 0):"
                           L"println(\"on the x-axis with an x value of \(x)\")"
                           L"case (0, let y):"
                           L"println(\"on the y-axis with a y value of \(y)\")"
                           L"case let (x, y):"
                           L"println(\"somewhere else at (\(x), \(y))\")"
                           L"}");
        
    }
    void testSwitch_Where()
    {
        PARSE_STATEMENT(L"switch yetAnotherPoint {"
                           L"case let (x, y) where x == y:"
                           L"println(\"(\(x), \(y)) is on the line x == y\")"
                           L"case let (x, y) where x == -y:"
                           L"println(\"(\(x), \(y)) is on the line x == -y\")"
                           L"case let (x, y):"
                           L"println(\"(\(x), \(y)) is just some arbitrary point\")"
                           L"}");
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        StatementPtr st;
        TuplePtr tuple;
        std::shared_ptr<ValueBinding> vb;
        BinaryOperatorPtr eq;
        UnaryOperatorPtr u;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(3, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(vb = std::dynamic_pointer_cast<ValueBinding>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
        //where
        CPPUNIT_ASSERT(eq = std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));
        ASSERT_EQUALS(L"==", eq->getOperator().c_str());
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(vb = std::dynamic_pointer_cast<ValueBinding>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
        //where
        CPPUNIT_ASSERT(eq = std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));
        ASSERT_EQUALS(L"==", eq->getOperator().c_str());
        CPPUNIT_ASSERT(u = std::dynamic_pointer_cast<UnaryOperator>(eq->getRHS()));
        CPPUNIT_ASSERT_EQUAL(OperatorType::PrefixUnary, u->getType());
        ASSERT_EQUALS(L"-", u->getOperator().c_str());
        
        CPPUNIT_ASSERT(c = sc->getCase(2));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(vb = std::dynamic_pointer_cast<ValueBinding>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
        CPPUNIT_ASSERT(NULL == std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));

        
    }
    
    void testSwitch_Enum()
    {
        PARSE_STATEMENT(L"switch directionToHead { "
                           L"case .North: "
                           L"println(\"Lots of planets have a north\") "
                           L"case .South: "
                           L"println(\"Watch out for penguins\") "
                           L"case .East: "
                           L"println(\"Where the sun rises\") "
                           L"case .West: "
                           L"println(\"Where the skies are blue\") "
                           L"}");
        
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        EnumCasePatternPtr e;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(4, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"North", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"South", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(2));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"East", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(3));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"West", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
    }
    
    void testSwitch_AssociatedValues()
    {
        PARSE_STATEMENT(L"switch productBarcode { "
                           L"case .UPCA(let numberSystem, let identifier, let check): "
                           L"println(\"UPC-A with value of \(numberSystem), \(identifier), \(check).\") "
                           L"case .QRCode(productCode): "
                           L"println(\"QR code with value of \(productCode).\") "
                           L"}");
        
        SwitchCasePtr sc;
        CaseStatementPtr c;
        EnumCasePatternPtr e;
        TuplePtr t;
        LetBindingPtr let;
        IdentifierPtr id;
        CPPUNIT_ASSERT(sc = std::dynamic_pointer_cast<SwitchCase>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"UPCA", e->getName().c_str());
        CPPUNIT_ASSERT(t = e->getAssociatedBinding());
        CPPUNIT_ASSERT_EQUAL(3, t->numElements());
        
        CPPUNIT_ASSERT(let = std::dynamic_pointer_cast<LetBinding>(t->getElement(0)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
        ASSERT_EQUALS(L"numberSystem", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(let = std::dynamic_pointer_cast<LetBinding>(t->getElement(1)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
        ASSERT_EQUALS(L"identifier", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(let = std::dynamic_pointer_cast<LetBinding>(t->getElement(2)));
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
        ASSERT_EQUALS(L"check", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"QRCode", e->getName().c_str());
        CPPUNIT_ASSERT(t = e->getAssociatedBinding());
        CPPUNIT_ASSERT_EQUAL(1, t->numElements());
        
        
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(t->getElement(0)));
        ASSERT_EQUALS(L"productCode", id->getIdentifier().c_str());
        
    }
    
    void testLabeledWhile()
    {
        PARSE_STATEMENT(L"gameLoop: while square != finalSquare {"
                           L"break gameLoop "
                           L"continue gameLoop"
                           L"}");
        LabeledStatementPtr label;
        WhileLoopPtr w;
        BinaryOperatorPtr op;
        CodeBlockPtr cb;
        
        CPPUNIT_ASSERT(label = std::dynamic_pointer_cast<LabeledStatement>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = std::dynamic_pointer_cast<WhileLoop>(label->getStatement()));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
        CPPUNIT_ASSERT(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
        ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
        CPPUNIT_ASSERT(bs);
        CPPUNIT_ASSERT(cs);
        ASSERT_EQUALS(L"gameLoop", bs->getLoop().c_str());
        ASSERT_EQUALS(L"gameLoop", cs->getLoop().c_str());
        
        
        
    }
    void testLabeledDo()
    {
        
        PARSE_STATEMENT(L"gameLoop: do {"
                           L"break gameLoop "
                           L"continue gameLoop"
                           L"} while square != finalSquare");
        LabeledStatementPtr label;
        DoLoopPtr w;
        BinaryOperatorPtr op;
        CodeBlockPtr cb;
        
        CPPUNIT_ASSERT(label = std::dynamic_pointer_cast<LabeledStatement>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = std::dynamic_pointer_cast<DoLoop>(label->getStatement()));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
        CPPUNIT_ASSERT(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
        ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
        CPPUNIT_ASSERT(bs);
        CPPUNIT_ASSERT(cs);
        ASSERT_EQUALS(L"gameLoop", bs->getLoop().c_str());
        ASSERT_EQUALS(L"gameLoop", cs->getLoop().c_str());
        
        
    }
    void testLabeledFor()
    {
        PARSE_STATEMENT(L"gameLoop: for ;square != finalSquare; {"
                           L"break gameLoop "
                           L"continue gameLoop"
                           L"}");
        LabeledStatementPtr label;
        ForLoopPtr w;
        BinaryOperatorPtr op;
        CodeBlockPtr cb;
        
        CPPUNIT_ASSERT(label = std::dynamic_pointer_cast<LabeledStatement>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = std::dynamic_pointer_cast<ForLoop>(label->getStatement()));
        CPPUNIT_ASSERT(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
        CPPUNIT_ASSERT(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
        ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
        CPPUNIT_ASSERT(bs);
        CPPUNIT_ASSERT(cs);
        ASSERT_EQUALS(L"gameLoop", bs->getLoop().c_str());
        ASSERT_EQUALS(L"gameLoop", cs->getLoop().c_str());
        
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestStatement, "alltest");



#endif
