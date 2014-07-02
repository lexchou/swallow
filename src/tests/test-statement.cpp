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
    CPPUNIT_TEST(testLabeledWhile);
    CPPUNIT_TEST(testLabeledDo);
    CPPUNIT_TEST(testLabeledFor);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testFor()
    {
        Node* root = parse(L"for i = 0;i < 10; i++{a = i * 2;println(a);}");
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
        
        DESTROY(root);
    }
    
    void testFor2()
    {
        Node* root = parse(L"for ;; {}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoop* loop = dynamic_cast<ForLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(0, loop->numInit());
        
        CPPUNIT_ASSERT(loop->getCondition() == NULL);
        CPPUNIT_ASSERT(loop->getStep() == NULL);
        
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(0, body->numStatements());
        
        DESTROY(root);
        
    }
    void testForIn()
    {
        
    }
    void testWhile()
    {
        Node* root = parse(L"while notTrue {println(a);}");
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
        
        DESTROY(root);
    }
    void testDo()
    {
        Node* root = parse(L"do{println(a);}while read(a)");
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
        
        DESTROY(root);
    }
    void testIf()
    {
        Node* root = parse(L"if a<0 {return -1}");
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
        
        DESTROY(root);
    }
    
    void testIf2()
    {
        Node* root = parse(L"if a<0 {return -1}else{return 1;}");
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
        
        DESTROY(root);
    }
    
    void testIf3()
    {
        Node* root = parse(L"if a<0 {return -1}else if a == 0 {return 0;} else {return 1}");
        
        
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
        
        DESTROY(root);
    }
    void testSwitch_Empty()
    {
        Node* root = parse(L"switch i{}");
        SwitchCase* sc = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(0, sc->numCases());
        CPPUNIT_ASSERT(sc->getDefaultCase() == NULL);

        DESTROY(root);
    }
    
    void testSwitch_MultipleCases()
    {
        Node* root = parse(L"switch someCharacter{"
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
        
        DESTROY(root);
    }
    
    void testSwitch_NoExplicitFallthrough()
    {
        Node* root = parse(L"switch anotherCharacter {"
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
        
        
        DESTROY(root);
    }
    
    void testSwitch_RangeMatching()
    {
        Node* root = parse(L"switch count {"
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
        
        DESTROY(root);
        
    }
    
    void testSwitch_Tuple()
    {
        
        Node* root = parse(L"switch somePoint {"
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
        
        
        DESTROY(root);
    }
    void testSwitch_ValueBindings()
    {
        Node* root = parse(L"switch anotherPoint {"
                           L"case (let x, 0):"
                           L"println(\"on the x-axis with an x value of \(x)\")"
                           L"case (0, let y):"
                           L"println(\"on the y-axis with a y value of \(y)\")"
                           L"case let (x, y):"
                           L"println(\"somewhere else at (\(x), \(y))\")"
                           L"}");
        
        DESTROY(root);
    }
    void testSwitch_Where()
    {
        Node* root = parse(L"switch yetAnotherPoint {"
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

        
        DESTROY(root);
    }
    
    void testLabeledWhile()
    {
        
    }
    void testLabeledDo()
    {
        
    }
    void testLabeledFor()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestStatement, "alltest");



#endif