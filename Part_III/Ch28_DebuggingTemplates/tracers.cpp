#include <iostream>
#include <algorithm>
#include <utility>


class SortTracer
{
private:
    int value_{0};                          // integer value to be sorted
    int generation_{1};                     // generation of this tracer
    inline static long n_created = 0;       // number of constructor calls
    inline static long n_destroyed = 0;     // number of dectructor calls
    inline static long n_assigned = 0;      // number of assignments
    inline static long n_compared = 0;      // number of comparisons
    inline static long n_max_live = 0;      // maximum number of objects

    // recompute maximum of existing objects
    static void update_max_live() noexcept {
        if (n_created - n_destroyed > n_max_live) {
            n_max_live = n_created - n_destroyed;
        }
    }

public:
    SortTracer(int v)
        : value_{v}
        {
            ++n_created;
            update_max_live();
            std::cerr << "SortTracer #" << n_created
                << ", created generation " << generation_
                << " (total: " << n_created - n_destroyed << ") \n";
        }

    SortTracer(SortTracer const& b)
        : value_{b.value_}, generation_{b.generation_+1}
        {
            ++n_created;
            update_max_live();
            std::cerr << "SortTracer #" << n_created
                << ", copied as generation " << generation_
                << " (total: " << n_created - n_destroyed << ") \n";
        }

    ~SortTracer() noexcept
    {
        ++n_destroyed;
        update_max_live();
        std::cerr << "SortTracer generation " << generation_
            << " destroyed (total: "
            << n_created - n_destroyed << ") \n";
    }

    SortTracer& operator=(SortTracer const& b)
    {
        ++n_assigned;
        value_ = b.value_;
        std::cerr << "SortTracer assignment #" << n_assigned
            << " (generation " << generation_
            << " = " << b.generation_ << ") \n";
        return *this;
    }

    // comparison
    friend bool operator<(SortTracer const& a, SortTracer const& b)
    {
        ++SortTracer::n_compared;
        std::cerr << "SortTracer comparison #" << SortTracer::n_compared
            << " (generation " << a.generation_
            << " < " << b.generation_ << ") \n";
        return a.value_ < b.value_;
    }

    auto val() const noexcept { return value_; }

    static auto creations() noexcept { return n_created; }
    static auto destructions() noexcept { return n_destroyed; }
    static auto assignments() noexcept { return n_assigned; }
    static auto comparisons() noexcept { return n_compared; }
    static auto max_live() noexcept { return n_max_live; }
};


int main()
{
    // prepare sample input
    SortTracer input[] = {7,3,5,6,4,2,0,1,9,8};

    std::cerr << "initial sequence: ";
    for (int i=0; i<10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << '\n';

    const long created_at_start = SortTracer::creations();
    const long max_live_at_start = SortTracer::max_live();
    const long assigned_at_start = SortTracer::assignments();
    const long compared_at_start = SortTracer::comparisons();

    // execute algorithm:
    std::cerr <<"---[ Start std::sort() ]------------------------------------------------------\n";
    std::sort(std::begin(input), std::end(input));
    std::cerr <<"---[ End std::sort() ]--------------------------------------------------------\n";

    std::cerr << "\nSorted sequence: ";
    for (int i = 0; i < 10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << "\n\n";

    // final report
    std::cerr << "std::sort() of 10 SortTracer's was performed by:\n "
        << SortTracer::creations() - created_at_start << " temporary tracers\n "
        << "up to "
        << SortTracer::max_live() << " tracers at the same time ("
        << max_live_at_start << " before)\n "
        << SortTracer::assignments() - assigned_at_start << " assignemnts\n "
        << SortTracer::comparisons() - compared_at_start << " comparisons\n\n";
}
