#pragma once

#include <cstddef>
#include <cassert>
#include "simple_array.hpp"
#include "expr_types.hpp"


template<typename T, typename Rep = SArray<T>>
class Array
{
private:
    Rep expr_rep_;  // (access to) the data of the array

public:
    // create array with initial size
    explicit Array(std::size_t s)
        : expr_rep_{s} { }

    // create array from possible representation
    Array(Rep const& rb)
        : expr_rep_{rb} { }

    // assignment operator for same type
    Array& operator= (Array const& b) {
        assert(size() == b.size());
        for (std::size_t idx = 0; idx < b.size(); ++idx) {
            expr_rep_[idx] = b[idx];
        }
        return *this;
    }

    // assignment operator for arrays of different type
    // We must take into account that the "other array" is really built on an expression template.
    template<typename T2, typename Rep2>
    Array& operator= (Array<T2, Rep2> const& b) {
        assert(size() == b.size());
        for (std::size_t idx = 0; idx < b.size(); ++idx) {
            expr_rep_[idx] = b[idx];
        }
        return *this;
    }

    // size is size of represented data
    std::size_t size() const { return expr_rep_.size(); }

    // index operator for constants and variables
    // decltype(auto) will correctly deduce a non reference type for Arrays where
    // Rep is A_Mult or A_Add since theri subscirpt operator retunrs a prvalue (temporary),
    // which can not be returned by reference. For Arrays where Rep = SArray<T>,
    // const lvalue reference will be deduced.
    decltype(auto) operator[] (std::size_t idx) const {
        assert(idx<size());
        return expr_rep_[idx];
    }

    T& operator[] (std::size_t idx) {
        assert(idx<size());
        return expr_rep_[idx];
    }

    // subscript operator to enable assignable expression subscripts
    template<typename T2, typename R2>
    Array<T, A_Subscript<T, Rep, R2>>
    operator[](Array<T2, R2> const& b)
    {
        return Array<T, A_Subscript<T, Rep, R2>>
                { A_Subscript<T, Rep, R2>{rep(), b.rep()} };
    }

    // return what the array currently represents
    Rep const& rep() const { return expr_rep_; }
    Rep& rep() { return expr_rep_; }
};
