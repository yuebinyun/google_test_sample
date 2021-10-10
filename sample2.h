#ifndef GOOGLE_TEST_SAMPLE_MYSTRING_H
#define GOOGLE_TEST_SAMPLE_MYSTRING_H

#include <string.h>

class MyString {
private:
    const char *c_string_;

    const MyString &operator=(const MyString &rhs);

public:
    // Clones a 0-terminated C string, allocating memory using new.
    static const char *CloneCString(const char *a_c_string);

    MyString() : c_string_(nullptr) {}

    explicit MyString(const char *a_c_string) : c_string_(nullptr) {
        Set(a_c_string);
    }

    MyString(const MyString &string) : c_string_(nullptr) {
        Set(string.c_string_);
    }

    ~MyString() { delete[] c_string_; }

    const char *c_string() const { return c_string_; }

    size_t Length() const { return c_string_ == nullptr ? 0 : strlen(c_string_); }

    void Set(const char *a_c_string);
};


#endif //GOOGLE_TEST_SAMPLE_MYSTRING_H
