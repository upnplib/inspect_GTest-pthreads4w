// Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2021-09-23

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <pthread.h>

#ifdef GTEST_IS_THREADSAFE
#  pragma message ( "pthread is available" )
#else
#  pragma message ( "pthread is NOT available" )
#endif

// simple mocked class
// -------------------
class Foo {
    virtual int GetSize() const = 0;
};

class MockFoo : public Foo {
  public:
    MOCK_METHOD(int, GetSize, (), (const, override));
};


// simple testsuite
//-----------------
TEST(simpleTestSuite, simpleMockTest) {
    MockFoo mockedFoo;

    EXPECT_CALL(mockedFoo, GetSize()).Times(1);
    EXPECT_EQ(mockedFoo.GetSize(), 0);
}


void* pthread_start_routine(void*) {
    return nullptr;
}

TEST(simpleTestSuite, simplePthreadsTest) {
    pthread_t thread;
    void* retval;

    EXPECT_EQ(pthread_create(&thread, NULL, &pthread_start_routine, NULL), 0);
    EXPECT_EQ(pthread_join(thread, &retval), 0);
    EXPECT_EQ(retval, (void*)NULL);
}


// main entry
// ----------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
