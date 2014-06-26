#ifndef TEST_STATEMENT_H
#define TEST_STATEMENT_H

#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

using namespace Swift;

class TestStatement : public CppUnit::TestFixture
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
    CPPUNIT_TEST(testSwitch);
    CPPUNIT_TEST(testSwitch2);
    CPPUNIT_TEST(testSwitch3);
    CPPUNIT_TEST(testLabeledWhile);
    CPPUNIT_TEST(testLabeledDo);
    CPPUNIT_TEST(testLabeledFor);
    CPPUNIT_TEST_SUITE_END();
public:
    
    Node* parse(const wchar_t* str)
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        return parser.parse(str);
    }
    
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
        
        
    }
    void testSwitch()
    {
        Node* root = parse(L"switch i{}");
        SwitchCase* sc = NULL;
        CPPUNIT_ASSERT(sc = dynamic_cast<SwitchCase*>(root));
        CPPUNIT_ASSERT_EQUAL(0, sc->numCases());
    }
    
    void testSwitch1()
    {
        Node* root = parse(L"switch i{}");
        
    }
    
    void testSwitch2()
    {
        Node* root = parse(L"switch i{}");
        
    }
    
    void testSwitch3()
    {
        Node* root = parse(L"switch i{}");
        
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