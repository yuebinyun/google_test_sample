#include "prime_tables.h"

#include "gtest/gtest.h"

namespace {
    template<class T>
    PrimeTable *CreatePrimeTable();

    template<>
    PrimeTable *CreatePrimeTable<OnTheFlyPrimeTable>() {
        return new OnTheFlyPrimeTable;
    }

    template<>
    PrimeTable *CreatePrimeTable<PreCalculatedPrimeTable>() {
        return new PreCalculatedPrimeTable(10000);
    }

    template<class T>
    class PrimeTableTest : public testing::Test {
    protected:
        PrimeTableTest() : table_(CreatePrimeTable<T>()) {}

        ~PrimeTableTest() override { delete table_; }

        PrimeTable *const table_;
    };

    using testing::Types;

    // Google Test offers two ways for reusing tests for different types.
    // The first is called "typed test". You should use it if you
    // already know *all* the types you are gonna exercise when you write
    // the tests.

    // To write a typed test case, first use
    //
    // TYPED_TEST_SUITE(TestCaseName, TypeList);
    //
    // to declare it and specify the type parameters. As with TEST_F
    // TestCaseName must match the test fixture name.

    // The list of types we want to test.
    typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> Implementations;
    TYPED_TEST_SUITE(PrimeTableTest, Implementations);

    TYPED_TEST(PrimeTableTest, ReturnsFalseForNonPrimes) {
        // Since we are in the template world, C++ requires explicitly
        // writing 'this->' when referring to members of the fixture class.
        // This is something you have to learn to live with.
        EXPECT_FALSE(this->table_->IsPrime(-5));
        EXPECT_FALSE(this->table_->IsPrime(0));
        EXPECT_FALSE(this->table_->IsPrime(1));
        EXPECT_FALSE(this->table_->IsPrime(4));
        EXPECT_FALSE(this->table_->IsPrime(100));
    }

    TYPED_TEST(PrimeTableTest, ReturnsTrueForPrimes) {
        EXPECT_TRUE(this->table_->IsPrime(2));
        EXPECT_TRUE(this->table_->IsPrime(3));
        EXPECT_TRUE(this->table_->IsPrime(5));
        EXPECT_TRUE(this->table_->IsPrime(7));
        EXPECT_TRUE(this->table_->IsPrime(11));
        EXPECT_TRUE(this->table_->IsPrime(131));
    }

    TYPED_TEST(PrimeTableTest, CanGetNextPrime) {
        EXPECT_EQ(2, this->table_->GetNextPrime(0));
        EXPECT_EQ(3, this->table_->GetNextPrime(2));
        EXPECT_EQ(5, this->table_->GetNextPrime(3));
        EXPECT_EQ(7, this->table_->GetNextPrime(5));
        EXPECT_EQ(11, this->table_->GetNextPrime(7));
        EXPECT_EQ(131, this->table_->GetNextPrime(128));
    }

    using testing::Types;

    template<class T>
    class PrimeTableTest2 : public PrimeTableTest<T> {
    };

    TYPED_TEST_SUITE_P(PrimeTableTest2);

    TYPED_TEST_P(PrimeTableTest2, ReturnsFalseForNonPrimes) {
        EXPECT_FALSE(this->table_->IsPrime(-5));
        EXPECT_FALSE(this->table_->IsPrime(0));
        EXPECT_FALSE(this->table_->IsPrime(1));
        EXPECT_FALSE(this->table_->IsPrime(4));
        EXPECT_FALSE(this->table_->IsPrime(6));
        EXPECT_FALSE(this->table_->IsPrime(100));
    }

    TYPED_TEST_P(PrimeTableTest2, ReturnsTrueForPrimes) {
        EXPECT_TRUE(this->table_->IsPrime(2));
        EXPECT_TRUE(this->table_->IsPrime(3));
        EXPECT_TRUE(this->table_->IsPrime(5));
        EXPECT_TRUE(this->table_->IsPrime(7));
        EXPECT_TRUE(this->table_->IsPrime(11));
        EXPECT_TRUE(this->table_->IsPrime(131));
    }

    TYPED_TEST_P(PrimeTableTest2, CanGetNextPrime) {
        EXPECT_EQ(2, this->table_->GetNextPrime(0));
        EXPECT_EQ(3, this->table_->GetNextPrime(2));
        EXPECT_EQ(5, this->table_->GetNextPrime(3));
        EXPECT_EQ(7, this->table_->GetNextPrime(5));
        EXPECT_EQ(11, this->table_->GetNextPrime(7));
        EXPECT_EQ(131, this->table_->GetNextPrime(128));
    }

    REGISTER_TYPED_TEST_SUITE_P(
            PrimeTableTest2,
            ReturnsFalseForNonPrimes,
            ReturnsTrueForPrimes,
            CanGetNextPrime);

    typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable>
            PrimeTableImplementations;

    INSTANTIATE_TYPED_TEST_SUITE_P(OnTheFlayAndPreCalculated,
                                   PrimeTableTest2,
                                   PrimeTableImplementations);
}
