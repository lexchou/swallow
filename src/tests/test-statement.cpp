#ifndef TEST_STATEMENT_H
#define TEST_STATEMENT_H

#include "utils.h"

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
        PARSE_STATEMENT(L"for i = 0;i < 10; i++{a = i * 2;println(a);}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoop* loop = dynamic_cast<ForLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(1, loop->numInit());
        Assignment* eq = dynamic_cast<Assignment*>(loop->getInit(0));
        CPPUNIT_ASSERT(eq != NULL);
        
        BinaryOperator* le = dynamic_cast<BinaryOperator*>(loop->getCondition());
        ASSERT_EQUALS(L"<", le->getOperator().c_str());
        CPPUNIT_ASSERT(le != NULL);
        
        UnaryOperator* step = dynamic_cast<UnaryOperator*>(loop->getStep());
        CPPUNIT_ASSERT(step != NULL);
        ASSERT_EQUALS(L"++", step->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, step->getType());
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(2, body->numStatements());
        CPPUNIT_ASSERT(eq = dynamic_cast<Assignment*>(body->getStatement(0)));
        FunctionCall* call;
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(body->getStatement(1)));
        Identifier* func;
        CPPUNIT_ASSERT(func = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    
    void testFor2()
    {
        PARSE_STATEMENT(L"for ;; {}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoop* loop = dynamic_cast<ForLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(0, loop->numInit());
        
        CPPUNIT_ASSERT(loop->getCondition() == NULL);
        CPPUNIT_ASSERT(loop->getStep() == NULL);
        
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(0, body->numStatements());
        
        
    }
    void testForIn()
    {
        PARSE_STATEMENT(L"for index in 1...5 {\n"
                        L"println(\"\(index) times 5 is \(index * 5)\")\n"
                        L"}");
        ForInLoop* f = NULL;
        BinaryOperator* op = NULL;
        Identifier* id = NULL;
        
        CPPUNIT_ASSERT(f = dynamic_cast<ForInLoop*>(root));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(f->getLoopVars()));
        ASSERT_EQUALS(L"index", id->getIdentifier());
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(f->getContainer()));
        ASSERT_EQUALS(L"...", op->getOperator());
        
        
        
        
    }
    void testWhile()
    {
        PARSE_STATEMENT(L"while notTrue {println(a);}");
        CPPUNIT_ASSERT(root != NULL);
        WhileLoop* loop = dynamic_cast<WhileLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        Identifier* cond = dynamic_cast<Identifier*>(loop->getCondition());
        CPPUNIT_ASSERT(cond != NULL);
        ASSERT_EQUALS(L"notTrue", cond->getIdentifier().c_str());
        
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        FunctionCall* call;
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(body->getStatement(0)));
        Identifier* func;
        CPPUNIT_ASSERT(func = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    void testDo()
    {
        PARSE_STATEMENT(L"do{println(a);}while read(a)");
        CPPUNIT_ASSERT(root != NULL);
        DoLoop* loop = dynamic_cast<DoLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        FunctionCall* call = dynamic_cast<FunctionCall*>(loop->getCondition());
        CPPUNIT_ASSERT(call != NULL);
        Identifier* func = static_cast<Identifier*>(call->getFunction());
        ASSERT_EQUALS(L"read", func->getIdentifier().c_str());
        
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(1, body->numStatements());
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(body->getStatement(0)));
        CPPUNIT_ASSERT(func = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
        
    }
    void testIf()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}");
        IfStatement* _if;
        BinaryOperator* op;
        CPPUNIT_ASSERT(_if = dynamic_cast<IfStatement*>(root));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatement* ret;
        CodeBlock* then;
        CPPUNIT_ASSERT(then = dynamic_cast<CodeBlock*>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(then->getStatement(0)));
        IntegerLiteral* id;
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->toString().c_str());
        CPPUNIT_ASSERT(_if->getElse() == NULL);
        
    }
    
    void testIf2()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}else{return 1;}");
        IfStatement* _if;
        BinaryOperator* op;
        CPPUNIT_ASSERT(_if = dynamic_cast<IfStatement*>(root));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatement* ret;
        CodeBlock* then, * elsePart;
        
        CPPUNIT_ASSERT(then = dynamic_cast<CodeBlock*>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(then->getStatement(0)));
        IntegerLiteral* id;
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->toString().c_str());
        
        
        CPPUNIT_ASSERT(elsePart = dynamic_cast<CodeBlock*>(_if->getElse()));
        CPPUNIT_ASSERT_EQUAL(1, elsePart->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(elsePart->getStatement(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"1", id->toString().c_str());
        
    }
    
    void testIf3()
    {
        PARSE_STATEMENT(L"if a<0 {return -1}else if a == 0 {return 0;} else {return 1}");
        
        
        IfStatement* _if;
        BinaryOperator* op;
        CPPUNIT_ASSERT(_if = dynamic_cast<IfStatement*>(root));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(_if->getCondition()));
        ASSERT_EQUALS(L"<", op->getOperator().c_str());
        ReturnStatement* ret;
        CodeBlock* then, * elsePart;
        
        CPPUNIT_ASSERT(then = dynamic_cast<CodeBlock*>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(then->getStatement(0)));
        IntegerLiteral* id;
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"-1", id->toString().c_str());
        
        
        CPPUNIT_ASSERT(_if = dynamic_cast<IfStatement*>(_if->getElse()));
        
        CPPUNIT_ASSERT(then = dynamic_cast<CodeBlock*>(_if->getThen()));
        CPPUNIT_ASSERT_EQUAL(1, then->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(then->getStatement(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"0", id->toString().c_str());
        
        
        CPPUNIT_ASSERT(elsePart = dynamic_cast<CodeBlock*>(_if->getElse()));

        CPPUNIT_ASSERT_EQUAL(1, elsePart->numStatements());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(elsePart->getStatement(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<IntegerLiteral*>(ret->getExpression()));
        ASSERT_EQUALS(L"1", id->toString().c_str());
        
    }
    void testSwitch_Empty()
    {
        PARSE_STATEMENT(L"switch i{}");
        SwitchCase* sc = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
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
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        Statement* st = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(5, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(st));
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(3, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(st));
        
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
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        Statement* st = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(0, c->numStatements());

        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<FunctionCall*>(st));
        
        
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
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        Statement* st = NULL;
        Pattern* p = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() != NULL);
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(st));
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(p = c->getCondition(0).condition);
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(st));
        
        CPPUNIT_ASSERT(c = sc->getDefaultCase());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(dynamic_cast<Assignment*>(st));
        
        
    }
    
    void testSwitch_Tuple()
    {
        
        PARSE_STATEMENT(L"switch somePoint {"
                           L"case (0, 0):"
                           L"println(\"(0, 0) is at the origin\")"
                           L"case (_, 0):"
                           L"println(\"(\(somePoint.0), 0) is on the x-axis\")"
                           L"}");
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        Statement* st = NULL;
        Tuple* tuple = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(dynamic_cast<IntegerLiteral*>(tuple->getElement(0)));
        CPPUNIT_ASSERT(dynamic_cast<IntegerLiteral*>(tuple->getElement(1)));
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(0)));
        CPPUNIT_ASSERT(dynamic_cast<IntegerLiteral*>(tuple->getElement(1)));
        
        
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
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        Statement* st = NULL;
        Tuple* tuple = NULL;
        ValueBinding* vb = NULL;
        BinaryOperator* eq;
        UnaryOperator* u = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(3, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(vb = dynamic_cast<ValueBinding*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(0)));
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(1)));
        //where
        CPPUNIT_ASSERT(eq = dynamic_cast<BinaryOperator*>(c->getCondition(0).guard));
        ASSERT_EQUALS(L"==", eq->getOperator().c_str());
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(vb = dynamic_cast<ValueBinding*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(0)));
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(1)));
        //where
        CPPUNIT_ASSERT(eq = dynamic_cast<BinaryOperator*>(c->getCondition(0).guard));
        ASSERT_EQUALS(L"==", eq->getOperator().c_str());
        CPPUNIT_ASSERT(u = dynamic_cast<UnaryOperator*>(eq->getRHS()));
        CPPUNIT_ASSERT_EQUAL(OperatorType::PrefixUnary, u->getType());
        ASSERT_EQUALS(L"-", u->getOperator().c_str());
        
        CPPUNIT_ASSERT(c = sc->getCase(2));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT_EQUAL(1, c->numStatements());
        CPPUNIT_ASSERT(st = c->getStatement(0));
        CPPUNIT_ASSERT(vb = dynamic_cast<ValueBinding*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(tuple = dynamic_cast<Tuple*>(vb->getBinding()));
        CPPUNIT_ASSERT_EQUAL(2, tuple->numElements());
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(0)));
        CPPUNIT_ASSERT(dynamic_cast<Identifier*>(tuple->getElement(1)));
        CPPUNIT_ASSERT(NULL == dynamic_cast<BinaryOperator*>(c->getCondition(0).guard));

        
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
        
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        EnumCasePattern* e = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(4, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"North", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"South", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(2));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"East", e->getName().c_str());
        CPPUNIT_ASSERT(e->getAssociatedBinding() == NULL);
        
        
        CPPUNIT_ASSERT(c = sc->getCase(3));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
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
        
        SwitchCase* sc = NULL;
        CaseStatement* c = NULL;
        EnumCasePattern* e = NULL;
        Tuple* t = NULL;
        LetBinding* let = NULL;
        Identifier* id = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(2, sc->numCases());
        
        CPPUNIT_ASSERT(c = sc->getCase(0));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"UPCA", e->getName().c_str());
        CPPUNIT_ASSERT(t = e->getAssociatedBinding());
        CPPUNIT_ASSERT_EQUAL(3, t->numElements());
        
        CPPUNIT_ASSERT(let = dynamic_cast<LetBinding*>(t->getElement(0)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(let->getBinding()));
        ASSERT_EQUALS(L"numberSystem", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(let = dynamic_cast<LetBinding*>(t->getElement(1)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(let->getBinding()));
        ASSERT_EQUALS(L"identifier", id->getIdentifier().c_str());
        
        CPPUNIT_ASSERT(let = dynamic_cast<LetBinding*>(t->getElement(2)));
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(let->getBinding()));
        ASSERT_EQUALS(L"check", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(c = sc->getCase(1));
        CPPUNIT_ASSERT_EQUAL(1, c->numConditions());
        CPPUNIT_ASSERT(e = dynamic_cast<EnumCasePattern*>(c->getCondition(0).condition));
        CPPUNIT_ASSERT(NULL == c->getCondition(0).guard);
        ASSERT_EQUALS(L"QRCode", e->getName().c_str());
        CPPUNIT_ASSERT(t = e->getAssociatedBinding());
        CPPUNIT_ASSERT_EQUAL(1, t->numElements());
        
        
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(t->getElement(0)));
        ASSERT_EQUALS(L"productCode", id->getIdentifier().c_str());
        
    }
    
    void testLabeledWhile()
    {
        PARSE_STATEMENT(L"gameLoop: while square != finalSquare {"
                           L"break gameLoop "
                           L"continue gameLoop"
                           L"}");
        LabeledStatement* label = NULL;
        WhileLoop* w = NULL;
        BinaryOperator* op = NULL;
        CodeBlock* cb = NULL;
        
        CPPUNIT_ASSERT(label = dynamic_cast<LabeledStatement*>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = dynamic_cast<WhileLoop*>(label->getStatement()));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(w->getCondition()));
        CPPUNIT_ASSERT(cb = dynamic_cast<CodeBlock*>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatement* bs = dynamic_cast<BreakStatement*>(cb->getStatement(0));
        ContinueStatement* cs = dynamic_cast<ContinueStatement*>(cb->getStatement(1));
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
        LabeledStatement* label = NULL;
        DoLoop* w = NULL;
        BinaryOperator* op = NULL;
        CodeBlock* cb = NULL;
        
        CPPUNIT_ASSERT(label = dynamic_cast<LabeledStatement*>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = dynamic_cast<DoLoop*>(label->getStatement()));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(w->getCondition()));
        CPPUNIT_ASSERT(cb = dynamic_cast<CodeBlock*>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatement* bs = dynamic_cast<BreakStatement*>(cb->getStatement(0));
        ContinueStatement* cs = dynamic_cast<ContinueStatement*>(cb->getStatement(1));
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
        LabeledStatement* label = NULL;
        ForLoop* w = NULL;
        BinaryOperator* op = NULL;
        CodeBlock* cb = NULL;
        
        CPPUNIT_ASSERT(label = dynamic_cast<LabeledStatement*>(root));
        ASSERT_EQUALS(L"gameLoop", label->getLabel().c_str());
        CPPUNIT_ASSERT(w = dynamic_cast<ForLoop*>(label->getStatement()));
        CPPUNIT_ASSERT(op = dynamic_cast<BinaryOperator*>(w->getCondition()));
        CPPUNIT_ASSERT(cb = dynamic_cast<CodeBlock*>(w->getCodeBlock()));
        ASSERT_EQUALS(L"!=", op->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(2, cb->numStatements());
        BreakStatement* bs = dynamic_cast<BreakStatement*>(cb->getStatement(0));
        ContinueStatement* cs = dynamic_cast<ContinueStatement*>(cb->getStatement(1));
        CPPUNIT_ASSERT(bs);
        CPPUNIT_ASSERT(cs);
        ASSERT_EQUALS(L"gameLoop", bs->getLoop().c_str());
        ASSERT_EQUALS(L"gameLoop", cs->getLoop().c_str());
        
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestStatement, "alltest");



#endif
