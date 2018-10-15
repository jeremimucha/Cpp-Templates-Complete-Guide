#include <iostream>
#include <type_traits>
#include <array>
#include <iterator>


// Empty Base Class Optimization - inheriting from an empty base class does not increase
// object's size.
// Creating generic patterns using this feature is problematic due to some restrictions
// - inheriting from non-class types or union types isn't allowed
// - inheriting from the same type more than once isn't allowed
// - inheriting from a `final` class isn't allowed

// A more practical pattern can be applied when there's a known member of a type that's known
// to be a class and at least one more member is available.
template<typename, typename> class BaseMemberPair;

template<typename CustomClass>
class Optimizable
{
private:
    BaseMemberPair<CustomClass, void*> info_and_storage_;
// instead of:
//  CustomClass info_;
//  void* storage_;
};

// This adds quite a bit of complexity to the implementation.
// A very basic BaseMemberPair implementation:
template<typename Base, typename Member>
class BaseMemberPair : private Base
{
private:
    Member mem;
public:
    BaseMemberPair(const Base& b, const Member& m)
        : Base{b}, mem{m} { }

// access base class via base()
    const Base& base() const { return static_cast<const Base&>(*this); }
    Base& base() { return static_cast<Base&>(*this); }
// acccess member via member()
    const Member& member() const { return this->mem; }
    Member& member() { return this->mem; }
};


// Basics of EBCO
// EBCO takes effect when the base class is `empty`, meaning that it doesn't have any non-static
// members, virtual functions and virtual base classes. Therefore static data, type members,
// non-virtual function members are fine.
class Empty {
    using Int = int;
};

class EmptyToo : public Empty {
};

class EmptyThree : public EmptyToo {
};

// A suprising, at first, limitation is that inheriting from two empty base classes
// makes the inheriting class non-empty. This is because the subobjects of the classes we inherit
// must have a unique addresses.
class NotEmpty : public Empty, public EmptyToo {
};




int main()
{
    std::cout << "sizeof(Empty): " << sizeof(Empty) << '\n';
    std::cout << "sizeof(EmptyToo): " << sizeof(EmptyToo) << '\n';
    std::cout << "sizeof(EmptyThree): " << sizeof(EmptyThree) << '\n';
    std::cout << "sizeof(NotEmpty): " << sizeof(NotEmpty) << '\n';
}
