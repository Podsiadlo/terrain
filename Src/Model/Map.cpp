#include <cstdio>
#include "Map.h"

double **Map::init_map_data(size_t rows, size_t cols) {
    double **map;
    map = (double **)malloc(rows * sizeof(double *));
    for (size_t i = 0; i < rows; ++i) {
        map[i] = (double *)malloc(cols * sizeof(double));
    }
    return map;
}

void Map::print_map() {
    for (int i = 0; i < this->length; ++i) {
        for (int j = 0; j < this->width; ++j) {
            fprintf(stdout, "%5.0lf ", this->data[i][j]);
        }
        fprintf(stdout, "\n");
    }
}


Map::~Map() {
    for (size_t i = 0; i < this->length; ++i) {
        free((double *) this->data[i]);
    }
    free(this->data);
}
