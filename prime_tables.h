#ifndef GOOGLE_TEST_SAMPLE_PRIME_TABLES_H
#define GOOGLE_TEST_SAMPLE_PRIME_TABLES_H

// This provides interface PrimeTable that determines whether a number is a
// prime and determines a next prime number. This interface is used
// in Google Test samples demonstrating use of parameterized tests

#include <algorithm>

class PrimeTable {
public:
    virtual ~PrimeTable() {};

    virtual bool IsPrime(int p) const = 0;

    virtual int GetNextPrime(int p) const = 0;
};

class OnTheFlyPrimeTable : public PrimeTable {
public :
    bool IsPrime(int n) const override {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if ((n % i) == 0) return false;
        }
        return true;
    }

    int GetNextPrime(int p) const override {
        if (p < 0) return -1;
        for (int n = p + 1;; n++) {
            if (IsPrime(n)) return n;
        }
    }
};

class PreCalculatedPrimeTable : public PrimeTable {
public:
    explicit PreCalculatedPrimeTable(int max) : is_prime_size_(max + 1), is_prime_(new bool[max + 1]) {
        CalculatePrimesUpTo(max);
    }

    ~PreCalculatedPrimeTable() {
        delete[] is_prime_;
    }

    bool IsPrime(int n) const override {
        return 0 <= n && n < is_prime_size_ && is_prime_[n];
    }

    int GetNextPrime(int p) const override {
        for (int n = p + 1; n < is_prime_size_; n++) {
            if (is_prime_[n]) return n;
        }
        return -1;
    }

private:
    void CalculatePrimesUpTo(int max) {
        ::std::fill(is_prime_, is_prime_ + is_prime_size_, true);
        is_prime_[0] = is_prime_[1] = false;
        for (int i = 2; i * i <= max; i += i % 2 + 1) {
            if (!is_prime_[i]) continue;
            for (int j = i * i; j < max; j += i) {
                is_prime_[j] = false;
            }
        }
    }

    const int is_prime_size_;
    bool *const is_prime_;

    void operator=(const PreCalculatedPrimeTable &rhs);

};

#endif //GOOGLE_TEST_SAMPLE_PRIME_TABLES_H
