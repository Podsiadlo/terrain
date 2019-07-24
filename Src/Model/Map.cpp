#include <cstdio>
#include <utility>
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
    if (with_data) {
        for (size_t i = 0; i < this->length; ++i) {
            free((double *) this->data[i]);
        }
        free(this->data);
    }
}

double Map::getValue(std::pair<double, double> coordinates)  {
    return getValue(coordinates.first, coordinates.second);
}

double Map::getValue(double latitude, double longitude) {
    if (with_data) {
        int x = (longitude - west_border) / cell_width;
        int y = (north_border - latitude) / cell_length;
        if (x == width) --x;
        if (y == length) --y;
        return data[y][x];
    } else {
        return reader->read_single_value(latitude, longitude);
    }
}
