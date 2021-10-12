#include <stdio.h>
#include "sample4.h"

int Counter::Increment() {
    return counter_++;
}

int Counter::Decrement() {
    if (counter_ == 0) {
        return counter_;
    } else {
        return counter_--;
    }
}

void Counter::Print() const {
    printf("%d\n", counter_);
}