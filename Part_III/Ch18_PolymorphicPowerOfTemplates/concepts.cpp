#include <iostream>
#include <vector>
#include <utility>


using Point = std::pair<int, int>;

Point operator-(Point&& x, const Point& y) noexcept
{
    x.first -= y.first;
    x.second -= y.second;
    return std::move(x);
}

Point operator-(const Point& x, const Point& y)
{
    auto x1 = x;
    return std::move(x1) - y;
}


template<typename T>
concept bool GeoObj = requires(T x) {
    { x.draw() } -> void;
    { x.center_of_gravity() } -> Point;
};

// concrete geometric object class Circle - not derived from anything
class Circle
{
public:
    void draw() const { std::cout << "draw() a circle\n"; }
    Point center_of_gravity() const { return {1,2}; }
    // ...
};

// concrete geometric object class Line - not derived from anything
class Line
{
public:
    void draw() const { std::cout << "draw() a line\n"; }
    Point center_of_gravity() const { return {3,4}; }
    // ...
};

// draw any GeoObj
template<typename T>
  requires GeoObj<T>
void myDraw(const T& obj)
{
    obj.draw(); // call draw() according to type of object
}

// compute distance of center of gravity between two GeoObjs
template<typename T1, typename T2>
  requires GeoObj<T1> && GeoObj<T2>
auto distance(const T1& x, const T2& y)
{
    auto p = x.center_of_gravity() - y.center_of_gravity();
    p.first = p.first < 0 ? -p.first : p.first;
    p.second = p.second < 0 ? -p.second : p.second;
    return p;
}

template<typename T>
  requires GeoObj<T>
void drawElems(std::vector<T> const& elems)
{
    for (auto&& e : elems) {
        e.draw();   // call draw() acording to type of element
    }
}

int main()
{
    Line l1;
    Circle c, c1, c2;

    myDraw(l1);         // myDraw<Line>() -> Line::draw();
    myDraw(c);          // myDraw<Circle>() -> Circle::draw();

    distance(c1,c2);    // distance<Circle,Circle>
    distance(l1,c);     // distance<Line,Circle>

    // std::vector<GeoObj*> coll;       // Error: no heterogeneous collection possible
    std::vector<Line> coll;             // OK: homogeneous collection is fine
    coll.push_back(l1);
    drawElems(coll);
}
