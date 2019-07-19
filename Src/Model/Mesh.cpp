#include <cstdio>
#include "Mesh.h"


void Mesh::prepare_mesh(size_t requested_size, bool use_height) {
    double cell_width;
    double cell_length;
    size_t rows;
    size_t columns;

    if (requested_size > (this->map->getWidth() - 1) || requested_size > (this->map->getLength() - 1)) {
        requested_size = this->map->getWidth() > this->map->getLength() ? (this->map->getLength() - 1)
                                                                        : (this->map->getWidth() - 1);
    }

    columns = (this->map->getWidth() - 1) / (requested_size / 2);
    if ((this->map->getWidth() - 1) % requested_size != 0) {
        if ((double) ((this->map->getWidth() - 1) % requested_size) / requested_size > 0.5) {
            columns++;
        }
    }
    cell_width = (double) (this->map->getWidth() - 1) / columns;

    rows = (this->map->getLength() - 1) / (requested_size / 3);
    if ((this->map->getLength() - 1) % requested_size != 0) {
        if ((double) ((this->map->getLength() - 1) % requested_size) / requested_size > 0.5) {
            rows++;
        }
    }
    cell_length = (double) (this->map->getLength() - 1) / rows;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            generate_first_triangles((int) (i * columns + j), cell_length, cell_width, columns, rows, use_height);
        }
    }
}

/*  ___
 * |2 /|
 * | / |
 * |/ 1|
 *  ———
 */
void Mesh::generate_first_triangles(int square_no, double cell_length, double cell_width, size_t cols, size_t rows,
                                    bool use_height) {
    size_t square_row = square_no / cols;
    size_t square_col = square_no % cols;
    cell_length = cell_length * this->map->getCellLength();
    cell_width = cell_width * this->map->getCellWidth();
    double first_data_row = this->map->getNorthBorder() - square_row * cell_length;
    double first_data_col = this->map->getWestBorder() + square_col * cell_width;

    Triangle *first = get_new_triangle();
    int first_index = first->getIndex();
    init_triangle(first_data_col, first_data_row - cell_length,
                  first_data_col + cell_width, first_data_row - cell_length,
                  first_data_col + cell_width, first_data_row, first, this->map, use_height);
    Triangle *second = get_new_triangle();
    init_triangle(first_data_col + cell_width, first_data_row,
                  first_data_col, first_data_row,
                  first_data_col, first_data_row - cell_length, second, this->map, use_height);

    first = get_triangle(first_index);
    first->getNeighbours()[2] = second->getIndex();
    first->getNeighbours()[1] = square_col == cols - 1 ? -1 : (square_no + 1) * 2 + 1;
    first->getNeighbours()[0] = square_row == rows - 1 ? -1 : (int) (square_no + cols) * 2 + 1;
    first->setIterations(0);
    second->getNeighbours()[2] = first->getIndex();
    second->getNeighbours()[1] = square_col % cols == 0 ? -1 : (square_no - 1) * 2;
    second->getNeighbours()[0] = square_row == 0 ? -1 : (int) (square_no - cols) * 2;
    second->setIterations(0);

    mark_border_points(first, second, square_col, square_row, cols, rows);
}

void Mesh::mark_border_points(Triangle *first_triangle, Triangle *second_triangle, size_t square_col,
                              size_t square_row, size_t total_cols, size_t total_rows) {
    if (square_row == total_rows - 1) { //south
        first_triangle->getVertices()[0].bitAndBorder(1 << 2);
        first_triangle->getVertices()[1].bitAndBorder(1 << 2);
        second_triangle->getVertices()[2].bitAndBorder(1 << 2);
    }
    if (square_col == total_cols - 1) { //east
        first_triangle->getVertices()[1].bitAndBorder(1 << 3);
        first_triangle->getVertices()[2].bitAndBorder(1 << 3);
        second_triangle->getVertices()[0].bitAndBorder(1 << 3);
    }
    if (square_row == 0) { //north
        first_triangle->getVertices()[2].bitAndBorder(1 << 4);
        second_triangle->getVertices()[0].bitAndBorder(1 << 4);
        second_triangle->getVertices()[1].bitAndBorder(1 << 4);
    }
    if (square_col == 0) { //west
        first_triangle->getVertices()[0].bitAndBorder(1 << 5);
        second_triangle->getVertices()[1].bitAndBorder(1 << 5);
        second_triangle->getVertices()[2].bitAndBorder(1 << 5);
    }
}


void Mesh::convert_mesh_to_UTM() {
    for (int i = 0; i < this->counter; ++i) {
        convert_triangle_to_UTM(&(this->triangles[i]), &(this->map->getZone()), &(this->map->getHemisphere()));
    }
    this->map->setUtm(true);
}

Triangle *Mesh::get_new_triangle() {
    if (this->counter >= this->size) {
        void *realloc_check = realloc(this->triangles, this->size * 2 * sizeof(Triangle));
        if (realloc_check == NULL) {
            fprintf(stderr, "Error during triangles reallocation.\n");
            exit(6);
        }
        this->triangles = static_cast<Triangle *>(realloc_check);
        this->size *= 2;
    }
    this->triangles[this->counter].setIndex(this->counter);
    return &(this->triangles[this->counter++]);
}

Triangle *Mesh::get_triangle(int index) {
    return index != -1 ? &(triangles[index]) : NULL;
}

Mesh::~Mesh() {
    delete this->map;
    free(this->triangles);
}


