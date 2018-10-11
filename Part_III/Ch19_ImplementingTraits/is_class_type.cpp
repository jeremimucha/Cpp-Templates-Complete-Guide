#include <iostream>
#include <type_traits>
#include <utility>

using std::void_t;


// It is only indirectly possible to detect if a type is a class-type, by utilizing a property
// that characterizes only classes. A convinient property to using is pointer to member types.
template<typename T, typename = void_t<>>
struct is_class : std::false_type { };

template<typename T>
struct is_class<T, void_t<int T::*>>    // int chosen arbitrarily
    : std::true_type { };               // only classes can have pointer-to-member


int main()
{

}
