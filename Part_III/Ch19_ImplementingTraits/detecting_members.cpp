#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

using std::void_t;
using std::remove_reference_t;

template<typename, typename = void_t<>>
struct has_size_type_bad : std::false_type { };

template<typename T>
struct has_size_type_bad<T, void_t<typename T::size_type>> : std::true_type { };

// We should always consider references...
// Calls to has_size_type_bad<std::string&> fail
// We can deal with it be removing references

template<typename, typename = void_t<>>
struct has_size_type : std::false_type { };

template<typename T>
struct has_size_type<T, void_t<typename remove_reference_t<T>::size_type>> : std::true_type { };


// We could use a macro to define a factory for similar has-type-member traits
// Or avoid the macro and endure the boilerplate for more explicitness and controll...
#define DEFINE_HAS_TYPE(MemType)    \
    template<typename, typename = void_t<>> \
    struct has_type_##MemType : std::false_type { }; \
    template<typename T> \
    struct has_type_##MemType<T, void_t<typename T::MemType>> : std::true_type { };

DEFINE_HAS_TYPE(value_type)
static_assert(has_type_value_type<std::string>::value, "has_type_value_type<string> test failed");


// We could use similar technique to detect non-type members
template<typename, typename = void_t<>>
struct has_member_size : std::false_type { };

template<typename T>
struct has_member_size<T, void_t<decltype(&T::size)>> : std::true_type { };

// This has some caveats:
// * Member must unambiguously indentify a member of T (e.g., it cannot be an overloaded
// member function name)
// * must be accessible
// * must be a non-type non-enumerator
// We could further specialize to allow only pointer to member data or pointer to member functions


// A simpler approach for detecting non-static members might be to use SFINAE's ability to detect
// invalid expressions
template<typename, typename = void_t<>>
struct has_begin : std::false_type { };

template<typename T>
struct has_begin<T, void_t<decltype(std::declval<T>().begin())>> : std::true_type { };

template<typename, typename, typename = void_t<>>
struct has_less : std::false_type { };

template<typename T1, typename T2>
struct has_less<T1, T2, void_t<decltype(
                               std::declval<T1>() < std::declval<T2>()
                               )>>
    : std::true_type { };


// Thanks to the properties of std::void_t we can detect multiple conditions at the same time
template<typename, typename = void_t<>>
struct has_various : std::false_type { };

template<typename T>
struct has_various<T, void_t<decltype(std::declval<T>().begin()),
                             typename T::difference_type,
                             typename T::iterator>>
    : std::true_type { };


int main()
{

}
