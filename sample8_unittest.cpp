// This sample shows how to test code relying on some global flag variables.
// Combine() helps with generating all possible combinations of such flags.
// and each test is given one combination as a parameter

#include "prime_tables.h"

#include "gtest/gtest.h"

namespace {

    class HybridPrimeTable : public PrimeTable {
    public:
        HybridPrimeTable(bool force_on_the_fly, int max_precalculated) :
                on_the_fly_impl_(new OnTheFlyPrimeTable),
                precalc_impl_(force_on_the_fly ? nullptr : new PreCalculatedPrimeTable(max_precalculated)),
                max_precalculated_(max_precalculated) {}

        ~HybridPrimeTable() override {
            delete on_the_fly_impl_;
            delete precalc_impl_;
        }

        bool IsPrime(int n) const override {
            if (precalc_impl_ != nullptr && n < max_precalculated_)
                return precalc_impl_->IsPrime(n);
            else
                return on_the_fly_impl_->IsPrime(n);
        }

        int GetNextPrime(int p) const override {
            int next_prime = -1;
            if (precalc_impl_ != nullptr && p < max_precalculated_)
                next_prime = precalc_impl_->GetNextPrime(p);

            return next_prime != -1 ? next_prime : on_the_fly_impl_->GetNextPrime(p);
        }

    private:
        OnTheFlyPrimeTable *on_the_fly_impl_;
        PreCalculatedPrimeTable *precalc_impl_;
        int max_precalculated_;
    };

    using ::testing::TestWithParam;
    using ::testing::Bool;
    using ::testing::Values;
    using ::testing::Combine;

    class PrimeTableTest : public TestWithParam<::std::tuple<bool, int>> {
    protected:
        void SetUp() override {
            bool force_on_the_fly;
            int max_precalculated;
            std::tie(force_on_the_fly, max_precalculated) = GetParam();
            table_ = new HybridPrimeTable(force_on_the_fly, max_precalculated);
        }

        void TearDown() override {
            delete table_;
            table_ = nullptr;
        }

        HybridPrimeTable *table_;
    };

    TEST_P(PrimeTableTest, ReturnsFalseForNonPrimes) {
        EXPECT_FALSE(table_->IsPrime(-5));
        EXPECT_FALSE(table_->IsPrime(0));
        EXPECT_FALSE(table_->IsPrime(1));
        EXPECT_FALSE(table_->IsPrime(4));
        EXPECT_FALSE(table_->IsPrime(6));
        EXPECT_FALSE(table_->IsPrime(10));
    }

    TEST_P(PrimeTableTest, ReturnsTrueForPrimes) {
        EXPECT_TRUE(table_->IsPrime(2));
        EXPECT_TRUE(table_->IsPrime(3));
        EXPECT_TRUE(table_->IsPrime(5));
        EXPECT_TRUE(table_->IsPrime(7));
        EXPECT_TRUE(table_->IsPrime(11));
        EXPECT_TRUE(table_->IsPrime(131));
    }

    TEST_P(PrimeTableTest, CanGetNextPrime) {
        EXPECT_EQ(2, table_->GetNextPrime(0));
        EXPECT_EQ(3, table_->GetNextPrime(2));
        EXPECT_EQ(5, table_->GetNextPrime(3));
        EXPECT_EQ(7, table_->GetNextPrime(5));
        EXPECT_EQ(11, table_->GetNextPrime(7));
        EXPECT_EQ(131, table_->GetNextPrime(128));
    }

    INSTANTIATE_TEST_SUITE_P(MeaningfulTestParamters,
                             PrimeTableTest,
                             Combine(Bool(), Values(1, 10)));
}