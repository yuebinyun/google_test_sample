#ifndef GOOGLE_TEST_SAMPLE_SAMPLE4_H
#define GOOGLE_TEST_SAMPLE_SAMPLE4_H

class Counter {
private:
    int counter_;

public:
    Counter() : counter_(0) {}

    int Increment();

    int Decrement();

    void Print() const;
};

#endif //GOOGLE_TEST_SAMPLE_SAMPLE4_H
