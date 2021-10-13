#include "prime_tables.h"
#include "gtest/gtest.h"

namespace {
    using ::testing::TestWithParam;
    using ::testing::Values;

    typedef PrimeTable *CreatePrimeTableFunc();

    PrimeTable *CreateOnTheFlyPrimeTable() {
        return new OnTheFlyPrimeTable();
    }

    template<size_t max_precalculated>
    PrimeTable *CreatePreCalculatedPrimeTable() {
        return new PreCalculatedPrimeTable(max_precalculated);
    }

    class PrimeTableTestSmpl7 : public TestWithParam<CreatePrimeTableFunc *> {
    public:
        ~PrimeTableTestSmpl7() override { delete table_; }

        void SetUp() override { table_ = (*GetParam())(); }

        void TearDown() override {
            delete table_;
            table_ = nullptr;
        }

    protected:
        PrimeTable *table_;
    };

    TEST_P(PrimeTableTestSmpl7, ReturnsFalseForNonPrimes) {
        EXPECT_FALSE(table_->IsPrime(-5));
        EXPECT_FALSE(table_->IsPrime(0));
        EXPECT_FALSE(table_->IsPrime(1));
        EXPECT_FALSE(table_->IsPrime(4));
        EXPECT_FALSE(table_->IsPrime(6));
        EXPECT_FALSE(table_->IsPrime(100));
    }

    TEST_P(PrimeTableTestSmpl7, ReturnsTrueForPrimes) {
        EXPECT_TRUE(table_->IsPrime(2));
        EXPECT_TRUE(table_->IsPrime(3));
        EXPECT_TRUE(table_->IsPrime(5));
        EXPECT_TRUE(table_->IsPrime(7));
        EXPECT_TRUE(table_->IsPrime(11));
        EXPECT_TRUE(table_->IsPrime(131));
    }

    TEST_P(PrimeTableTestSmpl7, CanGetNextPrime) {
        EXPECT_EQ(2, table_->GetNextPrime(0));
        EXPECT_EQ(3, table_->GetNextPrime(2));
        EXPECT_EQ(5, table_->GetNextPrime(3));
        EXPECT_EQ(7, table_->GetNextPrime(5));
        EXPECT_EQ(11, table_->GetNextPrime(7));
        EXPECT_EQ(131, table_->GetNextPrime(128));
    }

    INSTANTIATE_TEST_SUITE_P(OnTheTlyAndPreCalculated, PrimeTableTestSmpl7,
                             Values(&CreateOnTheFlyPrimeTable, &CreatePreCalculatedPrimeTable<1000>));
}
