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


class GeoObj
{
public:
    virtual void draw() const = 0;
    virtual Point center_of_gravity() const = 0;
    // ...
    virtual ~GeoObj() noexcept = default;
};

// concrete geometric object class Circle
class Circle : public GeoObj
{
public:
    void draw() const override { std::cout << "draw a cicrle\n"; }
    Point center_of_gravity() const override { return {1,2}; }
};

// concrete geometric object class Line
class Line : public GeoObj
{
public:
    void draw() const override { std::cout << "draw a line\n"; }
    Point center_of_gravity() const override { return {3,4}; }
};


// draw any GeoObj
void myDraw(const GeoObj& obj)
{
    obj.draw();
}

Point distance(const GeoObj& x1, const GeoObj& x2)
{
    auto p = x1.center_of_gravity() - x2.center_of_gravity();
    p.first = p.first < 0 ? -p.first : p.first;
    p.second = p.second < 0 ? -p.second : p.second;
    return p;
}

// draw heterogenneous collection of GeoObj
void drawElems(const std::vector<GeoObj*>& elems)
{
    for(const auto& e : elems) {
        e->draw();
    }
}

int main()
{
    Line l1;
    Circle c, c1, c2;

    myDraw(l1);         // calls Line::draw()
    myDraw(c);          // calls Circle::draw()

    distance(c1,c2);
    distance(l1,c);     // distance(GeoObj&,GeoObj&)

    std::vector<GeoObj*> coll;  // heterogeneous collection
    coll.push_back(&l1);        // insert line
    coll.push_back(&c);         // insert circle
    drawElems(coll);            // draw different kinds of GeoObjs
}
