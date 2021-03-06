#include "sample3_inl.h"
#include "gtest/gtest.h"

namespace {
    class QueueTestSmpl3 : public testing::Test {
    protected:

        // virtual void SetUp() will be called before each test is run. You
        // should define it if you need to initialize the variables.
        // Otherwise, this can be skipped.
        void SetUp() override {
            q1_.Enqueue(1);
            q2_.Enqueue(2);
            q2_.Enqueue(3);
        }

        static int Double(int n) {
            return 2 * n;
        }

        void MapTester(const Queue<int> *q) {
            const Queue<int> *const new_q = q->Map(Double);
            ASSERT_EQ(q->Size(), new_q->Size());

            for (const QueueNode<int> *n1 = q->Head(), *n2 = new_q->Head();
                 n1 != nullptr; n1 = n1->next(), n2 = n2->next()) {
                EXPECT_EQ(2 * n1->element(), n2->element());
            }

            delete new_q;
        }

        Queue<int> q0_;
        Queue<int> q1_;
        Queue<int> q2_;
    };

    TEST_F(QueueTestSmpl3, DefaultConstructor) {
        EXPECT_EQ(0u, q0_.Size());
    }

    TEST_F(QueueTestSmpl3, Dequeue) {
        int *n = q0_.Dequeue();
        EXPECT_TRUE(n == nullptr);

        n = q1_.Dequeue();
        ASSERT_TRUE(n != nullptr);
        EXPECT_EQ(1, *n);
        EXPECT_EQ(0u, q1_.Size());
        delete n;

        n = q2_.Dequeue();
        ASSERT_TRUE(n != nullptr);
        EXPECT_EQ(2, *n);
        EXPECT_EQ(1u, q2_.Size());
        delete n;
    }

    TEST_F(QueueTestSmpl3, Map) {
        MapTester(&q0_);
        MapTester(&q1_);
        MapTester(&q2_);
    }
}