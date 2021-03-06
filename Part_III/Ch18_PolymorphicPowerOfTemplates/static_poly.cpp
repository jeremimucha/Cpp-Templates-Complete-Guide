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

// draw any GeoObj (or any object with .draw())
template<typename GeoObj>
void myDraw(const GeoObj& obj)
{
    obj.draw(); // call draw() according to type of object
}

// compute distance of center of gravity between two GeoObjs
template<typename GeoObj1, typename GeoObj2>
auto distance(const GeoObj1& x, const GeoObj2& y)
{
    auto p = x.center_of_gravity() - y.center_of_gravity();
    p.first = p.first < 0 ? -p.first : p.first;
    p.second = p.second < 0 ? -p.second : p.second;
    return p;
}

template<typename GeoObj>
void drawElems(std::vector<GeoObj> const& elems)
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
