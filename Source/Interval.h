#ifndef INTERVAL_H
#define INTERVAL_H


class Interval
{
    public:
        float min;
        float max;

        static const Interval empty;
        static const Interval universe;

        Interval() : min(infinity), max(-infinity) {}
        Interval(float min, float max) : min(min), max(max) {}
        Interval(const Interval& a, const Interval& b)
        {
            min = (a.min <= b.min) ? a.min : b.min;
            max = (a.max >= b.max) ? a.max : b.max;
        }


        float size() const { return max - min; }

        // Closed interval; x = min or x = max ==> Interval contains x.
        bool contains(float x) const { return min <= x && x <= max; }

        // Open interval; same as contains() but without limit-values.
        bool surrounds(float x) const { return min < x && x < max; }

        float clamp(float x) const
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }

        Interval expand(float delta) const
        {
            float padding = delta / 2.0f;
            
            return Interval(min - padding, max + padding);
        }
};


const Interval Interval::empty = Interval(infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, infinity);

#endif
