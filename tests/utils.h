#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include "parser/Parser.h"
#include "ast/ast.h"
#include "ast/NodeFactory.h"
#include "common/CompilerResults.h"
#include "semantics/semantic-types.h"


#define ASSERT_EQUALS(E, A) wcs_assertEquals((E), (A), __FILE__, __LINE__)
#define ASSERT_NOT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) != NULL, #condition, false, true, GTEST_FATAL_FAILURE_)
#define ASSERT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) == NULL, #condition, false, true, GTEST_FATAL_FAILURE_)

void dumpCompilerResults(Swift::CompilerResults& compilerResults);
Swift::NodePtr parseStatement(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str);
Swift::ProgramPtr parseStatements(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str);

Swift::ScopedProgramPtr analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str);






struct Tracer
{
    Tracer(const char* file, int line, const char* func);
    ~Tracer();
    char file[1024];
    int line;
    char func[100];
    
};
#define PARSE_STATEMENT(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::CompilerResults compilerResults; \
    NodePtr root = parseStatement(compilerResults, __FUNCTION__, (s));




#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swift::SymbolRegistry symbolRegistry; \
    Swift::CompilerResults compilerResults; \
    ScopedProgramPtr root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    Swift::SymbolScope* scope = root ? root->getScope() : nullptr; \
    (void)scope;

#endif//TEST_UTILS_H



