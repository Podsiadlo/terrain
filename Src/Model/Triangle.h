#ifndef TERGEN_TRIANGLE_H
#define TERGEN_TRIANGLE_H


#include "Point.h"
#include "Map.h"

class Triangle {
private:
    int index;
    Point vertices[3]; // P0, P1, P2
    int neighbours[3]; // 0 -> P0P1, 1 -> P1P2, 2 -> P2P0
    short longest; // 0 -> P0P1, 1 -> P1P2, 2 -> P2P0
    double sides[3];
    int iterations;

public:
    void
    init_triangle(double a_x, double a_y, double b_x, double b_y, double c_x, double c_y, Map *map, bool use_height);

    void
    convert_triangle_to_UTM(long *zone, char *hemisphere);

    void
    fix_longest(bool use_height);

    void
    compute_sides(bool use_height);

    short
    choose_longest();

    double
    get_area(const struct point *a, const struct point *b, const struct point *c);

    void
    compute_barycentric_coords(double *barycentric_coords, struct point *point);

    bool
    is_inside_triangle(double barycentric_coords[]);

    double
    get_height_mean();

    double
    get_height_of_center(Map *map);

    int
    get_longest_edge_triangle_index();

    int
    get_1st_shorter_edge_triangle_index();

    int
    get_2nd_shorter_edge_triangle_index();

    void
    get_longest_edge_midsection(Point *destination);

    struct point *
    get_opposite_vertex();

    struct point *
    get_1st_longest_edge_vertex();

    struct point *
    get_2nd_longest_edge_vertex();


    int getIndex() const {
        return index;
    }

    void setIndex(int index) {
        Triangle::index = index;
    }

    const Point *getVertices() const {
        return vertices;
    }

    const int *getNeighbours() const {
        return neighbours;
    }

    short getLongest() const {
        return longest;
    }

    void setLongest(short longest) {
        Triangle::longest = longest;
    }

    const double *getSides() const {
        return sides;
    }

    int getIterations() const {
        return iterations;
    }

    void setIterations(int iterations) {
        Triangle::iterations = iterations;
    }
};


#endif //TERGEN_TRIANGLE_H
