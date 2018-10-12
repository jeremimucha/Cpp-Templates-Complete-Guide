#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>


// Class templates can also be enabled/disabled using partial specialization enable_if techniques.
template<typename Key, typename Value, typename = void>
class Dictionary
{
    // default implementation using a vector
};

template<typename Key, typename Value>
class Dictionary<Key, Value,
                 std::enable_if_t<has_less<Key> && !has_hash<Key>>>
{
    // implementation using a map
};

template<typename Key, typename Value>
class Dictionary<Key, Value,
                 std::enable_if_t<has_hash<Key>>>
{
    // implementation using unordered_map
};



int main()
{

}
