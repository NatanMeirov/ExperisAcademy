#ifndef __POINT_HPP__
#define __POINT_HPP__

struct Point {
    // C'tors
    // Default C'tor
    Point();
    //
    Point(double a_x, double a_y);

    // Destroy
    ~Point();

    // On self
    double DistanceToOrigin();
    void Print();

    // Between Points
    double DistanceTo(Point a_other);
    Point Add(Point a_other);
    Point Substact(Point a_other); // Better then passing by ref (only 16 bytes are copied comparing to 8 bytes of ptr, but dereferencing is more "expensive" then +8 bytes of copy)

    // Members
    double m_x;
    double m_y;
};

#endif // __POINT_HPP__