PROJECT(swift)
cmake_minimum_required(VERSION 2.6)

FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h
    /usr/local/include
    /usr/include
)
FIND_PATH(CPPUNIT_LIBRARIES libcppunit.a
    /usr/local/lib
    /usr/lib
)
LINK_DIRECTORIES(${CPPUNIT_LIBRARIES})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
    ${CPPUNIT_INCLUDE_DIR}
)


SET( CMAKE_BUILD_TYPE Debug )
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
cmake_policy(SET CMP0015 OLD)



SET(SWIFT_TOKENIZER_SRC tokenizer/tokenizer.cpp)
SET(SWIFT_SRC main.cpp tokenizer/tokenizer.cpp)


ADD_EXECUTABLE(TestTokenizer tests/test-tokenizer.cpp ${SWIFT_TOKENIZER_SRC})
target_link_libraries(TestTokenizer cppunit)
ADD_TEST(NAME TestTokenizer COMMAND TestTokenizer)

ADD_EXECUTABLE(swift ${SWIFT_SRC})
