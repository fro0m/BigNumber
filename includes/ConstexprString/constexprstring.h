#ifndef CONSTEXPRSTRING_H
#define CONSTEXPRSTRING_H

#include <cstddef>
#include <cassert>

class ConstexprString { // constexpr string
public:
    template<std::size_t N>
    constexpr ConstexprString(const char(&a)[N]) : // ctor
        p_(a), sz_(N - 1) {}
    constexpr char operator[](std::size_t n) const { // []
        assert(n < sz_);
        return p_[n];
    }
    constexpr std::size_t size() { return sz_; } // size()
    constexpr const char * rawData() const {
        return p_;
    }
private:
    const char* const p_;
    const std::size_t sz_;
};

#endif // CONSTEXPRSTRING_H
