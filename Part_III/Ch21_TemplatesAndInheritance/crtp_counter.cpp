#include <iostream>


// CRTP is one of the more usefull template-design-patterns.
// As an example it can be used to implement an instance counter generically.
template<typename CountedType>
class ObjectCounter
{
private:
    inline static std::size_t count{0};     // number of existing objects
protected:
    constexpr ObjectCounter() noexcept { ++count; }
    constexpr ObjectCounter(const ObjectCounter&) { ++count; }
    constexpr ObjectCounter(ObjectCounter&&) { ++count; }
    ~ObjectCounter() noexcept { --count; }
public:
    static constexpr std::size_t live() noexcept { return count; }
};

// Now any type we want to count the number instance of just has to inherit from
// ObjectCounter<The-Counted-Type>
template<typename CharT>
class MyString : public ObjectCounter<MyString<CharT>> {
};


int main()
{
    MyString<char> s1, s2;
    MyString<wchar_t> ws;
    std::cout << "num of MyString<char>: " << MyString<char>::live() << '\n';
    std::cout << "num of MyString<wchar_t>: " << ws.live() << '\n';
}
