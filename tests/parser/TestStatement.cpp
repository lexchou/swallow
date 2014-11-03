/* TestStatement.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "../utils.h"

using namespace Swift;
    
TEST(TestStatement, testFor)
{
    PARSE_STATEMENT(L"for i = 0;i < 10; i++ {a = i * 2;println(a);}");
    ASSERT_NOT_NULL(root);
    ForLoopPtr loop = std::dynamic_pointer_cast<ForLoop>(root);
    ASSERT_NOT_NULL(loop);

    ASSERT_EQ(1, loop->numInit());
    AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(loop->getInit(0));
    ASSERT_NOT_NULL(eq);

    BinaryOperatorPtr le = std::dynamic_pointer_cast<BinaryOperator>(loop->getCondition());
    ASSERT_EQ(L"<", le->getOperator());
    ASSERT_NOT_NULL(le);

    UnaryOperatorPtr step = std::dynamic_pointer_cast<UnaryOperator>(loop->getStep());
    ASSERT_NOT_NULL(step);
    ASSERT_EQ(L"++", step->getOperator());
    ASSERT_EQ(OperatorType::PostfixUnary, step->getOperatorType());

    CodeBlockPtr body = loop->getCodeBlock();

    ASSERT_EQ(2, body->numStatements());
    ASSERT_NOT_NULL(eq = std::dynamic_pointer_cast<Assignment>(body->getStatement(0)));
    FunctionCallPtr call;
    ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(1)));
    IdentifierPtr func;
    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
    ASSERT_EQ(L"println", func->getIdentifier());

}

TEST(TestStatement, testFor2)
{
    PARSE_STATEMENT(L"for ;; {}");
    ASSERT_NOT_NULL(root);
    ForLoopPtr loop = std::dynamic_pointer_cast<ForLoop>(root);
    ASSERT_NOT_NULL(loop);

    ASSERT_EQ(0, loop->numInit());

    ASSERT_NULL(loop->getCondition());
    ASSERT_NULL(loop->getStep());


    CodeBlockPtr body = loop->getCodeBlock();

    ASSERT_EQ(0, body->numStatements());


}
TEST(TestStatement, testForIn)
{
    PARSE_STATEMENT(L"for index in 1...5 {\n"
                    L"println(\"\(index) times 5 is \(index * 5)\")\n"
                    L"}");
    ForInLoopPtr f;
    BinaryOperatorPtr op;
    IdentifierPtr id;

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<ForInLoop>(root));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(f->getLoopVars()));
    ASSERT_EQ(L"index", id->getIdentifier());
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(f->getContainer()));
    ASSERT_EQ(L"...", op->getOperator());




}
TEST(TestStatement, testWhile)
{
    PARSE_STATEMENT(L"while notTrue {println(a);}");
    ASSERT_NOT_NULL(root);
    WhileLoopPtr loop = std::dynamic_pointer_cast<WhileLoop>(root);
    ASSERT_NOT_NULL(loop);

    IdentifierPtr cond = std::dynamic_pointer_cast<Identifier>(loop->getCondition());
    ASSERT_NOT_NULL(cond);
    ASSERT_EQ(L"notTrue", cond->getIdentifier());


    CodeBlockPtr body = loop->getCodeBlock();

    ASSERT_EQ(1, body->numStatements());
    FunctionCallPtr call;
    ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(0)));
    IdentifierPtr func;
    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
    ASSERT_EQ(L"println", func->getIdentifier());

}
TEST(TestStatement, testDo)
{
    PARSE_STATEMENT(L"do{println(a);}while read(a)");
    ASSERT_NOT_NULL(root);
    DoLoopPtr loop = std::dynamic_pointer_cast<DoLoop>(root);
    ASSERT_NOT_NULL(loop);

    FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(loop->getCondition());
    ASSERT_NOT_NULL(call);
    IdentifierPtr func = std::static_pointer_cast<Identifier>(call->getFunction());
    ASSERT_EQ(L"read", func->getIdentifier());


    CodeBlockPtr body = loop->getCodeBlock();

    ASSERT_EQ(1, body->numStatements());
    ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(body->getStatement(0)));
    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
    ASSERT_EQ(L"println", func->getIdentifier());

}
TEST(TestStatement, testIf)
{
    PARSE_STATEMENT(L"if a<0 {return -1}");
    IfStatementPtr _if;
    BinaryOperatorPtr op;
    ASSERT_NOT_NULL(_if = std::dynamic_pointer_cast<IfStatement>(root));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
    ASSERT_EQ(L"<", op->getOperator());
    ReturnStatementPtr ret;
    CodeBlockPtr then;
    ASSERT_NOT_NULL(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
    ASSERT_EQ(1, then->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
    IntegerLiteralPtr id;
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"-1", id->valueAsString);
    ASSERT_NULL(_if->getElse());

}

TEST(TestStatement, testIf2)
{
    PARSE_STATEMENT(L"if a<0 {return -1}else{return 1;}");
    IfStatementPtr _if;
    BinaryOperatorPtr op;
    ASSERT_NOT_NULL(_if = std::dynamic_pointer_cast<IfStatement>(root));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
    ASSERT_EQ(L"<", op->getOperator());
    ReturnStatementPtr ret;
    CodeBlockPtr then, elsePart;

    ASSERT_NOT_NULL(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
    ASSERT_EQ(1, then->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
    IntegerLiteralPtr id;
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"-1", id->valueAsString);


    ASSERT_NOT_NULL(elsePart = std::dynamic_pointer_cast<CodeBlock>(_if->getElse()));
    ASSERT_EQ(1, elsePart->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(elsePart->getStatement(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"1", id->valueAsString);

}

TEST(TestStatement, testIf3)
{
    PARSE_STATEMENT(L"if a<0 {return -1}else if a == 0 {return 0;} else {return 1}");


    IfStatementPtr _if;
    BinaryOperatorPtr op;
    ASSERT_NOT_NULL(_if = std::dynamic_pointer_cast<IfStatement>(root));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(_if->getCondition()));
    ASSERT_EQ(L"<", op->getOperator());
    ReturnStatementPtr ret;
    CodeBlockPtr then, elsePart;

    ASSERT_NOT_NULL(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
    ASSERT_EQ(1, then->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
    IntegerLiteralPtr id;
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"-1", id->valueAsString);


    ASSERT_NOT_NULL(_if = std::dynamic_pointer_cast<IfStatement>(_if->getElse()));

    ASSERT_NOT_NULL(then = std::dynamic_pointer_cast<CodeBlock>(_if->getThen()));
    ASSERT_EQ(1, then->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(then->getStatement(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"0", id->valueAsString);


    ASSERT_NOT_NULL(elsePart = std::dynamic_pointer_cast<CodeBlock>(_if->getElse()));

    ASSERT_EQ(1, elsePart->numStatements());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(elsePart->getStatement(0)));
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<IntegerLiteral>(ret->getExpression()));
    ASSERT_EQ(L"1", id->valueAsString);

}
TEST(TestStatement, testSwitch_Empty)
{
    PARSE_STATEMENT(L"switch i{}");
    SwitchCasePtr sc;
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(0, sc->numCases());
    ASSERT_NULL(sc->getDefaultCase());

}

TEST(TestStatement, testSwitch_MultipleCases)
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
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(2, sc->numCases());
    ASSERT_NOT_NULL(sc->getDefaultCase());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(5, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(st));

    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(3, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(st));

    ASSERT_NOT_NULL(c = sc->getDefaultCase());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(st));

}

TEST(TestStatement, testSwitch_NoExplicitFallthrough)
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
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(2, sc->numCases());
    ASSERT_NOT_NULL(sc->getDefaultCase());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(0, c->numStatements());

    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(st));

    ASSERT_NOT_NULL(c = sc->getDefaultCase());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(st));


}

TEST(TestStatement, testSwitch_RangeMatching)
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
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(2, sc->numCases());
    ASSERT_NOT_NULL(sc->getDefaultCase());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(st));

    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(p = c->getCondition(0).condition);
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(st));

    ASSERT_NOT_NULL(c = sc->getDefaultCase());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(st));


}

TEST(TestStatement, testSwitch_Tuple)
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
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(2, sc->numCases());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(c->getCondition(0).condition));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(1)));


    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(c->getCondition(0).condition));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<IntegerLiteral>(tuple->getElement(1)));


}
TEST(TestStatement, testSwitch_ValueBindings)
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
TEST(TestStatement, testSwitch_Where)
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
    ValueBindingPatternPtr vb;
    BinaryOperatorPtr eq;
    UnaryOperatorPtr u;
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(3, sc->numCases());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(vb = std::dynamic_pointer_cast<ValueBindingPattern>(c->getCondition(0).condition));
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
    //where
    ASSERT_NOT_NULL(eq = std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));
    ASSERT_EQ(L"==", eq->getOperator());


    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(vb = std::dynamic_pointer_cast<ValueBindingPattern>(c->getCondition(0).condition));
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
    //where
    ASSERT_NOT_NULL(eq = std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));
    ASSERT_EQ(L"==", eq->getOperator());
    ASSERT_NOT_NULL(u = std::dynamic_pointer_cast<UnaryOperator>(eq->getRHS()));
    ASSERT_EQ(OperatorType::PrefixUnary, u->getOperatorType());
    ASSERT_EQ(L"-", u->getOperator());

    ASSERT_NOT_NULL(c = sc->getCase(2));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_EQ(1, c->numStatements());
    ASSERT_NOT_NULL(st = c->getStatement(0));
    ASSERT_NOT_NULL(vb = std::dynamic_pointer_cast<ValueBindingPattern>(c->getCondition(0).condition));
    ASSERT_NOT_NULL(tuple = std::dynamic_pointer_cast<Tuple>(vb->getBinding()));
    ASSERT_EQ(2, tuple->numElements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(tuple->getElement(1)));
    ASSERT_NULL(std::dynamic_pointer_cast<BinaryOperator>(c->getCondition(0).guard));


}

TEST(TestStatement, testSwitch_Enum)
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
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(4, sc->numCases());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"North", e->getName());
    ASSERT_NULL(e->getAssociatedBinding());


    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"South", e->getName());
    ASSERT_NULL(e->getAssociatedBinding());


    ASSERT_NOT_NULL(c = sc->getCase(2));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"East", e->getName());
    ASSERT_NULL(e->getAssociatedBinding());


    ASSERT_NOT_NULL(c = sc->getCase(3));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"West", e->getName());
    ASSERT_NULL(e->getAssociatedBinding());

}

TEST(TestStatement, testSwitch_AssociatedValues)
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
    ValueBindingPatternPtr let;
    IdentifierPtr id;
    ASSERT_NOT_NULL(sc = std::dynamic_pointer_cast<SwitchCase>(root));
    ASSERT_EQ(2, sc->numCases());

    ASSERT_NOT_NULL(c = sc->getCase(0));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"UPCA", e->getName());
    ASSERT_NOT_NULL(t = e->getAssociatedBinding());
    ASSERT_EQ(3, t->numElements());

    ASSERT_NOT_NULL(let = std::dynamic_pointer_cast<ValueBindingPattern>(t->getElement(0)));
    ASSERT_TRUE(let->isReadOnly());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
    ASSERT_EQ(L"numberSystem", id->getIdentifier());

    ASSERT_NOT_NULL(let = std::dynamic_pointer_cast<ValueBindingPattern>(t->getElement(1)));
    ASSERT_TRUE(let->isReadOnly());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
    ASSERT_EQ(L"identifier", id->getIdentifier());

    ASSERT_NOT_NULL(let = std::dynamic_pointer_cast<ValueBindingPattern>(t->getElement(2)));
    ASSERT_TRUE(let->isReadOnly());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(let->getBinding()));
    ASSERT_EQ(L"check", id->getIdentifier());


    ASSERT_NOT_NULL(c = sc->getCase(1));
    ASSERT_EQ(1, c->numConditions());
    ASSERT_NOT_NULL(e = std::dynamic_pointer_cast<EnumCasePattern>(c->getCondition(0).condition));
    ASSERT_NULL(c->getCondition(0).guard);
    ASSERT_EQ(L"QRCode", e->getName());
    ASSERT_NOT_NULL(t = e->getAssociatedBinding());
    ASSERT_EQ(1, t->numElements());


    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(t->getElement(0)));
    ASSERT_EQ(L"productCode", id->getIdentifier());

}

TEST(TestStatement, testLabeledWhile)
{
    PARSE_STATEMENT(L"gameLoop: while square != finalSquare {"
                       L"break gameLoop "
                       L"continue gameLoop"
                       L"}");
    LabeledStatementPtr label;
    WhileLoopPtr w;
    BinaryOperatorPtr op;
    CodeBlockPtr cb;

    ASSERT_NOT_NULL(label = std::dynamic_pointer_cast<LabeledStatement>(root));
    ASSERT_EQ(L"gameLoop", label->getLabel());
    ASSERT_NOT_NULL(w = std::dynamic_pointer_cast<WhileLoop>(label->getStatement()));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
    ASSERT_NOT_NULL(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
    ASSERT_EQ(L"!=", op->getOperator());
    ASSERT_EQ(2, cb->numStatements());
    BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
    ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
    ASSERT_NOT_NULL(bs);
    ASSERT_NOT_NULL(cs);
    ASSERT_EQ(L"gameLoop", bs->getLoop());
    ASSERT_EQ(L"gameLoop", cs->getLoop());



}
TEST(TestStatement, testLabeledDo)
{

    PARSE_STATEMENT(L"gameLoop: do {"
                       L"break gameLoop "
                       L"continue gameLoop"
                       L"} while square != finalSquare");
    LabeledStatementPtr label;
    DoLoopPtr w;
    BinaryOperatorPtr op;
    CodeBlockPtr cb;

    ASSERT_NOT_NULL(label = std::dynamic_pointer_cast<LabeledStatement>(root));
    ASSERT_EQ(L"gameLoop", label->getLabel());
    ASSERT_NOT_NULL(w = std::dynamic_pointer_cast<DoLoop>(label->getStatement()));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
    ASSERT_NOT_NULL(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
    ASSERT_EQ(L"!=", op->getOperator());
    ASSERT_EQ(2, cb->numStatements());
    BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
    ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
    ASSERT_NOT_NULL(bs);
    ASSERT_NOT_NULL(cs);
    ASSERT_EQ(L"gameLoop", bs->getLoop());
    ASSERT_EQ(L"gameLoop", cs->getLoop());


}
TEST(TestStatement, testLabeledFor)
{
    PARSE_STATEMENT(L"gameLoop: for ;square != finalSquare; {"
                       L"break gameLoop "
                       L"continue gameLoop"
                       L"}");
    LabeledStatementPtr label;
    ForLoopPtr w;
    BinaryOperatorPtr op;
    CodeBlockPtr cb;

    ASSERT_NOT_NULL(label = std::dynamic_pointer_cast<LabeledStatement>(root));
    ASSERT_EQ(L"gameLoop", label->getLabel());
    ASSERT_NOT_NULL(w = std::dynamic_pointer_cast<ForLoop>(label->getStatement()));
    ASSERT_NOT_NULL(op = std::dynamic_pointer_cast<BinaryOperator>(w->getCondition()));
    ASSERT_NOT_NULL(cb = std::dynamic_pointer_cast<CodeBlock>(w->getCodeBlock()));
    ASSERT_EQ(L"!=", op->getOperator());
    ASSERT_EQ(2, cb->numStatements());
    BreakStatementPtr bs = std::dynamic_pointer_cast<BreakStatement>(cb->getStatement(0));
    ContinueStatementPtr cs = std::dynamic_pointer_cast<ContinueStatement>(cb->getStatement(1));
    ASSERT_NOT_NULL(bs);
    ASSERT_NOT_NULL(cs);
    ASSERT_EQ(L"gameLoop", bs->getLoop());
    ASSERT_EQ(L"gameLoop", cs->getLoop());


}
