#ifndef HASHING_H
#define HASHING_H

#include "CoreTypes.h"


// FNV-1a constants
static constexpr U64 basis = 14695981039346656037ULL;
static constexpr U64 prime = 1099511628211ULL;


// compile-time hash
constexpr U64 hash_char(char c, const char* remain, U64 value)
{
    return c == 0 ? value : hash_char(remain[0], remain + 1, (value ^ c) * prime);
}

// compile-time hash wrapper
constexpr U64 hash(const char* str)
{
    return hash_char(str[0], str + 1, basis);
}

// run-time hash
U64 hash_rt(const char* str)
{
    U64 hash = basis;
    while (*str != 0) {
        hash ^= str[0];
        hash *= prime;
        ++str;
    }
    return hash;
}

// Test compile time hash
template <U64 NUM>
struct test_const
{
    static const U64 value = NUM;
};

// test
int main()
{
    constexpr U64 value = test_const<hash ("hello, world!")>::value;
    U64 value_rt = hash_rt("hello, world!");

    std::cout < < "hash =    " << value << std::endl;
    std::cout << "hash_rt = " << value_rt << std::endl;

    return 0;
}

#endif // HASHING_H