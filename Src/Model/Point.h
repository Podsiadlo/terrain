#ifndef TERGEN_POINT_H
#define TERGEN_POINT_H


#include <cstdlib>

class Point {
private:

        double x;
        double y;
        double z;
        char border;

public:
    void
    init_point(struct point *point, double x, double y, struct map *map);

    double
    get_height(double x, double y, struct map *map);

    double
    get_height_wo_interpol(double lon, double lat, int corner, const struct map *map);

    bool
    point_equals(struct point *a, struct point *b);

    double
    get_coordinate(int coordinate, struct point *point);

    struct point *
    mul_by_scalar(double scalar, const struct point *point);

    void
    sort_points(int size, size_t indices[], int coordinate, struct point **points);

    double getX() const {
        return x;
    }

    void setX(double x) {
        Point::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        Point::y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        Point::z = z;
    }

    char getBorder() const {
        return border;
    }

    void setBorder(char border) {
        Point::border = border;
    }

    void bitAndBorder(char border) {
        Point::border |= border;
    }
};


#endif //TERGEN_POINT_H
