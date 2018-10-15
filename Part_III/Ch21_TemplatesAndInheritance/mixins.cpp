#include <iostream>
#include <string>
#include <vector>
#include <utility>


// Mixins provide an alternative way to customize the behavior of a type without inheriting from it.
// Instead the new classes are `mixed in` to the hierarchy as base classes of a class template,
// rather than being created as new derived classes.
template<typename... Mixins>
class Point : public Mixins...
{
public:
    double x, y;
    Point() : Mixins{}..., x{0.0}, y{0.0} { }
    Point(double xx, double yy) : Mixins{}..., x{xx}, y{yy} { }
};

// Now new base classes can be `mixed in`
class Label
{
public:
    std::string label;
    Label() : label{""} { }
};

class Color
{
public:
    unsigned char red{0}, green{0}, blue{0};
};

using MyPoint = Point<Label, Color>;


// Combining Mixins with CRTP results in an even more powerful pattern...
template<template<typename> class... Mixins>
class Point2 : public Mixins<Point2<Mixins>>...
{
public:
    double x, y;
    Point2() : Mixins<Point2>{}..., x{0.0}, y{0.0} { }
    Point2(double xx, double yy) : Mixins<Point2>{}..., x{xx}, y{yy} { }
};

// This forces the Mixin classes to become templates. However mixed-in classes can now adapt their
// behavior to the specific instance of a class they've been mixed into. For example we could
// mix in the ObjectCounter template into point...


// Mixins also allow us to provide a dynamicly polymorphic interface via inheriting from a class
// with virtual members

class NotVirtual { };

class Virtual {
public:
    virtual void foo() { }
};

template<typename... Mixins>
class Base : public Mixins...
{
public:
    // The virtuality of foo() depends on its declaration (if any) in the base classes Mixins...
    void foo() { std::cout << "Base::foo()" << '\n'; }
};

template<typename... Mixins>
class Derived : public Base<Mixins...>
{
public:
    void foo() { std::cout << "Derived::foo()" << '\n'; }
};

int main()
{
    Derived<NotVirtual> d1;
    static_cast<Base<NotVirtual>*>(&d1)->foo();    // calls Base::foo()

    Derived<Virtual> d2;
    static_cast<Base<Virtual>*>(&d2)->foo();       // calls Derived::foo()
}
