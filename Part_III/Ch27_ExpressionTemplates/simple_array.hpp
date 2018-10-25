#pragma once

#include <cstddef>
#include <cassert>


template<typename T>
class SArray
{
public:
    // create array with initial size
    explicit SArray(std::size_t s)
        : storage_{new T[s]}, storage_size_{s}
        {
            init();
        }
    
    // copy constructor
    SArray(SArray const& orig)
        : storage_{new T[orig.size()]}, storage_size_{orig.size()}
        {
            copy(orig);
        }

    // destructor
    ~SArray() {
        delete[] storage_;
    }

    // assignement
    SArray& operator=(SArray const& orig)
    {
        if (&orig != this) {
            copy(orig);
        }
        return *this;
    }

    // return size
    std::size_t size() const {
        return storage_size_;
    }

    // index operator for constants and variables
    T const& operator[](std::size_t idx) const {
        return storage_[idx];
    }

    T& operator[] (std::size_t idx) {
        return storage_[idx];
    }

protected:
    // init values with default constructor
    void init() {
        for (std::size_t idx = 0; idx < size(); ++idx) {
            storage_[idx] = T{};
        }
    }
    // copy values of another array
    void copy(SArray const& orig) {
        assert(size() == orig.size());
        for (std::size_t idx = 0; idx < size(); ++idx) {
            storage_[idx] = orig.storage_[idx];
        }
    }

private:
    T*          storage_;           // storage of the elements
    std::size_t storage_size_;      // number of elements
};

