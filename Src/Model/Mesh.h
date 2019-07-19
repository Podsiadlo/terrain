#ifndef TERGEN_MESH_H
#define TERGEN_MESH_H


#include <cstdlib>
#include "Map.h"
#include "Triangle.h"

class Mesh {
private:
    static const int INITIAL_MESH_SIZE = 50;

    Triangle *triangles;

    size_t size;

    size_t counter;

    Map *map;


    void prepare_mesh(size_t requested_size, bool use_height);

    void generate_first_triangles(int square_no, double cell_length, double cell_width, size_t cols, size_t rows,
                             bool use_height);

    void mark_border_points(Triangle *first_triangle, Triangle *second_triangle, size_t square_col,
                       size_t square_row, size_t total_cols, size_t total_rows);

    Triangle *get_new_triangle();

    Triangle *get_triangle(int index);

public:
    Mesh(Map *map, size_t requested_size, bool use_height) : map(map) {
        this->triangles = (Triangle *) malloc(INITIAL_MESH_SIZE * sizeof(Triangle));
        this->size = INITIAL_MESH_SIZE;
        this->counter = 0;
        this->map->setUtm(false);
        prepare_mesh(requested_size, use_height);
    };

    void convert_mesh_to_UTM();

    static const int getInitialMeshSize() {
        return INITIAL_MESH_SIZE;
    }

    Triangle *getTriangles() const {
        return triangles;
    }

    void setTriangles(Triangle *triangles) {
        Mesh::triangles = triangles;
    }

    size_t getSize() const {
        return size;
    }

    void setSize(size_t size) {
        Mesh::size = size;
    }

    size_t getCounter() const {
        return counter;
    }

    void setCounter(size_t counter) {
        Mesh::counter = counter;
    }

    Map *getMap() const {
        return map;
    }

    void setMap(Map *map) {
        Mesh::map = map;
    }

    virtual ~Mesh();
};


#endif //TERGEN_MESH_H
