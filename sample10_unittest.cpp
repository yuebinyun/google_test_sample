#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace {

    // We will track memory used by this class.
    class Water {
    public:
        // Normal Water declarations go here.

        // operator new and operator delete help us control water allocation.
        void *operator new(size_t allocation_size) {
            allocated_++;
            return malloc(allocation_size);
        }

        void operator delete(void *block, size_t) {
            allocated_--;
            free(block);
        }

        static int allocated() { return allocated_; }

    private:
        static int allocated_;
    };

    int Water::allocated_ = 0;

    // This event listener monitors how many Water objects are created and
    // destroyed by each test, and reports a failure if a test leaks some Water
    // objects. It does this by comparing the number of live Water objects at
    // the beginning of a test and at the end of a test.
    class LeakChecker : public EmptyTestEventListener {
    private:
        void OnTestStart(const TestInfo &) override {
            initially_allocated_ = Water::allocated();
        }

        void OnTestEnd(const TestInfo &) override {
            int difference = Water::allocated() - initially_allocated_;

            // You can generate a failure in any event handler except
            // OnTestParResult. Just use an appropriate Google Test assertion to do
            // it.
            EXPECT_LE(difference, 0) << "Leaked" << difference << " unit(s) of Water!";
        }

        int initially_allocated_;
    };


    TEST(ListenersTest, DoesNotLeak) {
        Water *water = new Water;
        delete water;
    }

    TEST(ListenersTest, LeaksWater) {
        Water *water = new Water;
        EXPECT_TRUE(water != nullptr);
    }
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);

    bool check_for_leaks = false;
    if (argc > 1 && strcmp(argv[1], "--check_for_leaks") == 0)
        check_for_leaks = true;
    else
        printf("%s\n", "Run this program with --check_for_leaks to enable "
                       "custom leak checking in the test.");

    if (check_for_leaks) {
        TestEventListeners &listeners = UnitTest::GetInstance()->listeners();
        listeners.Append(new LeakChecker);
    }
    return RUN_ALL_TESTS();
}

