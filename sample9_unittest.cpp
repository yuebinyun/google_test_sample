// This sample shows how to use Google Test listener API to implement
// an alternative console output and how to use the UnitTest reflection API
// to enumerate test cases and tests and to inspect their results.

#include <stdio.h>
#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace {
    // Provides alternative output mode which produces minimal amount of
    // information about tests.
    class TersePrinter : public EmptyTestEventListener {
    private:
        // Called before any test activity starts.
        void OnTestProgramStart(const UnitTest &) override {}

        // Called after all test activities have ended.
        void OnTestProgramEnd(const UnitTest &unit_test) override {
            fprintf(stdout, "TEST %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
            fflush(stdout);
        }

        // Called before a test starts.
        void OnTestStart(const TestInfo &test_info) override {
            fprintf(stdout,
                    "*** Test %s.%s starting.\n",
                    test_info.test_case_name(),
                    test_info.name());
            fflush(stdout);
        }

        // Called after a failed assertion or a SUCCEED() invocation.
        void OnTestPartResult(const TestPartResult &test_part_result) override {
            fprintf(stdout,
                    "%s in %s:%d\n%s\n",
                    test_part_result.failed() ? "*** Failure" : "Success",
                    test_part_result.file_name(),
                    test_part_result.line_number(),
                    test_part_result.summary());
            fflush(stdout);
        }

        // Called after a test ends.
        void OnTestEnd(const TestInfo &test_info) override {
            fprintf(stdout,
                    "*** Test %s.%s ending.\n",
                    test_info.test_case_name(),
                    test_info.name());
            fflush(stdout);
        }
    };

    TEST(CustomOutputTest, PrinsMessage) {
        printf("Printing something from the test body...\n");
    }

    TEST(CustomOutPutTest, Succeeds) {
        SUCCEED() << "SUCCEED() has been invoked from here";
    }

    TEST(CustomOutputTest, Fails) {
        EXPECT_EQ(1, 2) << "This test fails in order to demonstrate alternative failure message";
    }
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);

    bool terse_output = false;
    if (argc > 1 && strcmp(argv[1], "--terse_output") == 0)
        terse_output = true;
    else
        printf("%s\n", "Run this program with --terse_output to change the way it prints its output.");

    UnitTest &unit_test = *UnitTest::GetInstance();

    if (terse_output) {
        TestEventListeners &listeners = unit_test.listeners();

        delete listeners.Release(listeners.default_result_printer());

        listeners.Append(new TersePrinter);
    }

    int ret_val = RUN_ALL_TESTS();

    int unexpectedly_failed_tests = 0;
    for (int i = 0; i < unit_test.total_test_suite_count(); ++i) {
        const testing::TestSuite &test_suite = *unit_test.GetTestSuite(i);
        for (int j = 0; j < test_suite.total_test_count(); ++j) {
            const TestInfo &test_info = *test_suite.GetTestInfo(j);
            if (test_info.result()->Failed() && strcmp(test_info.name(), "Fails") != 0) {
                unexpectedly_failed_tests++;
            }
        }
    }

    if (unexpectedly_failed_tests == 0)
        ret_val = 0;
    return ret_val;
}

