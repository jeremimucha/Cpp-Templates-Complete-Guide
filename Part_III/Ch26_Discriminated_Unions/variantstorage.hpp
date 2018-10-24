#pragma once

#include <new>  // for std::launder
#include "typelist/typelist.hpp"
#include "typelist/largest_type.hpp"


template<typename... Types>
class VariantStorage
{
private:
    using largest_t = largest_type_t<typelist<Types...>>;

    alignas(Types...) unsigned char buffer_[sizeof(largest_t)];
    unsigned char discriminator_ = 0;

public:
    unsigned char getDiscriminator() const { return discriminator_; }
    void setDiscriminator(unsigned char d) { discriminator_ = d; }
    void* getRawBuffer() { return buffer_; }
    const void* getRawBuffer() const { return buffer_; }

    template<typename T>
    T* getBufferAs() { return std::launder(reinterpret_cast<T*>(buffer_)); }
    template<typename T>
    T const* getBufferAs() const { return std::launder(reinterpret_cast<T const*>(buffer_)); }
};
