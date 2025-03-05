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

        float size() const { return max - min; }

        // Closed interval; x = min or x = max ==> Interval contains x.
        bool contains(float x) const { return min <= x && x <= max; }

        // Open interval; same as contains() but without limit-values.
        bool surronds(float x) const { return min < x && x < max; }

        float clamp(float x) const
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }
};


const Interval Interval::empty = Interval(infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, infinity);

#endif
