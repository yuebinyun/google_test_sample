#include <limits.h>
#include <time.h>
#include "gtest/gtest.h"
#include "sample1.h"
#include "sample3_inl.h"

namespace {
    // In this sample, we want to ensure that every test finishes with
    // ~5 seconds. If a test takes longer to run, we consider it a failure.

    // We put the code for timing a test in a test fixture called "QuickTest".
    // QuickTest is intended to be the super fixture that other fixtures
    // derive from, therefore there is no test case with the name "QuickTest". This is OK.
    //
    // Later, we will derive multiple test fixtures from QuickTest.
    class QuickTest : public testing::Test {
    private:
        // THe UTC time(in seconds) when the test starts
        time_t start_time_;

    protected:
        // Remember that SetUp() is run immediately before a test starts.
        // This is a good place to record the start time.
        void SetUp() override { start_time_ = time(nullptr); }

        // TearDown() is invoked immediately after a test finishes.
        // Here we check if the test was too slow.
        void TearDown() override {
            const time_t end_time = time(nullptr);
            EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
        }
    };

    // We derive a fixture named IntegerFunctionTest from the QuickTest fixture.
    // All tests using this fixture will be automatically required to be quick.
    class IntegerFunctionTest : public QuickTest {

    };

    TEST_F(IntegerFunctionTest, Factorial) {
        EXPECT_EQ(1, Factorial(-5));
        EXPECT_EQ(1, Factorial(-1));
        EXPECT_GT(Factorial(-10), 0);

        EXPECT_EQ(1, Factorial(0));

        EXPECT_EQ(1, Factorial(1));
        EXPECT_EQ(2, Factorial(2));
        EXPECT_EQ(6, Factorial(3));
        EXPECT_EQ(40320, Factorial(8));
    }

    TEST_F(IntegerFunctionTest, IsPrime) {
        EXPECT_FALSE(IsPrime(1));
        EXPECT_FALSE(IsPrime(-2));
        EXPECT_FALSE(IsPrime(INT_MIN));

        EXPECT_FALSE(IsPrime(0));
        EXPECT_FALSE(IsPrime(1));
        EXPECT_TRUE(IsPrime(2));
        EXPECT_TRUE(IsPrime(3));

        EXPECT_FALSE(IsPrime(4));
        EXPECT_TRUE(IsPrime(5));
        EXPECT_FALSE(IsPrime(6));
        EXPECT_TRUE(IsPrime(23));
    }

    // The next test case (named "QueueTest") also needs to be quick, so quick,
    // so we derive another fixture form QuickTest.
    //
    // The QueueTest test fixture has some logic and shared objects in addition to what's in
    // QuickTest already. We define the additional stuff inside the body of
    // the test fixture, as usual.
    class QueueTest : public QuickTest {
    protected:
        Queue<int> q0_;
        Queue<int> q1_;
        Queue<int> q2_;

        void SetUp() override {
            // First, we need to set up the super fixture(QuickTest).
            QuickTest::SetUp();

            q1_.Enqueue(1);
            q2_.Enqueue(2);
            q2_.Enqueue(3);
        }
    };

    TEST_F(QueueTest, DefaultConstructor) {
        EXPECT_EQ(0u, q0_.Size());
    }

    TEST_F(QueueTest, Dequeu) {
        int *n = q0_.Dequeue();
        EXPECT_TRUE(n == nullptr);

        n = q1_.Dequeue();
        EXPECT_TRUE(n != nullptr);
        EXPECT_EQ(1, *n);
        EXPECT_EQ(0u, q1_.Size());

        n = q2_.Dequeue();
        EXPECT_TRUE(n != nullptr);
        EXPECT_EQ(2, *n);
        EXPECT_EQ(1u, q2_.Size());
        delete n;
    }
}
