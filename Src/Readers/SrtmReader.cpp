#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cmath>
#include "SrtmReader.h"
#include "../Utils/Utils.h"

//Map *SrtmReader::read_SRTM(const double west_border, const double north_border, const double east_border,
//                           const double south_border) { // data[row][column] - it's array of rows
//    Utils::swap_if_required((double *) &south_border, (double *) &north_border);
//    Utils::swap_if_required((double *) &west_border, (double *) &east_border);
//    // Rounding to avoid problems with numerical errors
//    int west_border_int = border_to_int(west_border);
//    int north_border_int = border_to_int(north_border);
//    int east_border_int = border_to_int(east_border);
//    int south_border_int = border_to_int(south_border);
//
//    size_t cols = (size_t) (east_border_int - west_border_int);
//    size_t rows = (size_t) (north_border_int - south_border_int);
//    double **map_data = Map::init_map_data(rows, cols);
//    Map *map = new Map(map_data, cols, rows, 1. / VALUES_IN_DEGREE, 1. / VALUES_IN_DEGREE);
//    map->setNorthBorder(north_border);
//    map->setWestBorder(west_border);
//
//    read_from_multiple_files(west_border, north_border, east_border, south_border, map_data);
//
//    skip_outliers(map_data, map->getLength(), map->getWidth());
//
//    return map;
//}

void SrtmReader::read_from_multiple_files(const double west_border, const double north_border, const double east_border,
                                          const double south_border, double **map_data) {
    int first_free_row = 0;
    double north_ptr = north_border;
    double south_ptr = Utils::is_lesser(Utils::floor2(north_border), south_border) ? south_border : Utils::floor2(
            north_border);
    if (Utils::equals(north_ptr, south_ptr)) {
        south_ptr = Utils::is_lesser(north_border - 1, south_border) ? south_border : north_border - 1;
    }
    while (Utils::is_greater(north_ptr, south_border)) {
        int north_ptr_int = border_to_int(north_ptr);
        size_t rows_here = (size_t) std::abs(north_ptr_int - border_to_int(south_ptr));

        int first_free_col = 0;
        double west_ptr = west_border;
        double east_ptr = Utils::is_greater(Utils::ceil2(west_border), east_border) ? east_border : Utils::ceil2(
                west_border);

        while (Utils::is_lesser(west_ptr, east_border)) {
            int west_ptr_int = border_to_int(west_ptr);
            size_t cols_here = (size_t) std::abs(border_to_int(east_ptr) - west_ptr_int);

            read_values(north_ptr_int, west_ptr_int, rows_here, cols_here, first_free_row, first_free_col, map_data);

            first_free_col += cols_here;
            west_ptr = Utils::floor2(west_ptr + 1);
            east_ptr = Utils::is_greater(east_ptr + 1, east_border) ? east_border : east_ptr + 1;
        }

        first_free_row += rows_here;
        north_ptr = Utils::equals(Utils::floor2(north_ptr), north_ptr) ? north_ptr - 1 : Utils::floor2(north_ptr);
        south_ptr = Utils::is_lesser(south_ptr - 1, south_border) ? south_border : south_ptr - 1;
    }
}

void SrtmReader::read_values(int north_border_int, int west_border_int, size_t rows, size_t cols, int first_row,
                             int first_col, double **map_data) {
    char file_to_open[256];
    get_filename(file_to_open, west_border_int, north_border_int);
    if (access(file_to_open, F_OK) != -1) { //checks whether file exists
        read_from_file(north_border_int, west_border_int, file_to_open, rows, cols, first_row, first_col, map_data);
    } else {
        read_zeros(rows, cols, first_row, first_col, map_data);
    }
}

void SrtmReader::get_filename(char *filename, int west_border_int, int north_border_int) {
    int first_long_to_read;
    int first_lat_to_read;

    west_border_int = ((west_border_int + 180 * VALUES_IN_DEGREE) % (360 * VALUES_IN_DEGREE)) - 180 * VALUES_IN_DEGREE;

    if (west_border_int < 0) {
        if (west_border_int % VALUES_IN_DEGREE != 0) {
            first_long_to_read = west_border_int / VALUES_IN_DEGREE + 1;
        } else {
            first_long_to_read = west_border_int / VALUES_IN_DEGREE;
        }
    } else {
        first_long_to_read = west_border_int / VALUES_IN_DEGREE;
    }

    if (north_border_int < 0) {
        if (north_border_int % VALUES_IN_DEGREE != 0) {
            first_lat_to_read = north_border_int / VALUES_IN_DEGREE + 1;
        } else {
            first_lat_to_read = north_border_int / VALUES_IN_DEGREE;
        }
    } else {
        if (north_border_int % VALUES_IN_DEGREE != 0) {
            first_lat_to_read = north_border_int / VALUES_IN_DEGREE;
        } else {
            first_lat_to_read = north_border_int / VALUES_IN_DEGREE - 1;
        }
    }

    sprintf(filename, "%s/%s%d%s%.3d.hgt", map_dir.data(),
            first_lat_to_read < 0 ? "S" : "N", first_lat_to_read,
            first_long_to_read < 0 ? "W" : "E", first_long_to_read);
}


void
SrtmReader::read_from_file(int north_border_int, int west_border_int, char *filename, size_t rows, size_t cols,
                           int first_row, int first_col, double **map_data) {

    FILE *map_file;
    if ((map_file = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    int cells_in_degree = VALUES_IN_DEGREE + 1;
    if (fseek(map_file,
              (((VALUES_IN_DEGREE - (north_border_int % VALUES_IN_DEGREE)) % VALUES_IN_DEGREE) * cells_in_degree +
               (west_border_int % VALUES_IN_DEGREE)) *
              PIXEL_SIZE,
              SEEK_SET) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    uint16_t *buffer = (uint16_t *) malloc(PIXEL_SIZE * cols);
    for (size_t i = 0; i < rows; ++i) {
        fread(buffer, PIXEL_SIZE, cols, map_file);
        if (fseek(map_file, (cells_in_degree - cols) * PIXEL_SIZE, SEEK_CUR) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
        for (size_t j = 0; j < cols; ++j) {
            Utils::change_bytes_order(&(buffer[j]));
            map_data[first_row + i][first_col + j] = buffer[j];
        }
    }
    free(buffer);
    if (fclose(map_file) != 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
}

void SrtmReader::read_zeros(size_t rows, size_t cols, int first_row, int first_col, double **map_data) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            map_data[first_row + i][first_col + j] = 0;
        }
    }
}

void SrtmReader::skip_outliers(double *const *map_data, size_t length, size_t width) { //TODO: Investigate
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            if (map_data[i][j] > 5000 || map_data[i][j] < 0) {
                printf("WARNING: Outliers detected. Skipping...\n");
                if (i > 0) {
                    map_data[i][j] = map_data[i - 1][j];
                } else {
                    map_data[i][j] = map_data[i + 1][j];
                }
            }
//            if (map_data[i][j] > 3000 || map_data[i][j] < 10) {
//                printf("WARNING: Outliers detected. Skipping...\n");
//                if (j > 0) {
//                    map_data[i][j] = map_data[i][j-1];
//                } else {
//                    map_data[i][j] = map_data[i][j+1];
//                }
//            }
        }
    }
}

int SrtmReader::border_to_int(const double border) const {
    return (int) round(border * SrtmReader::VALUES_IN_DEGREE);
}

double SrtmReader::read_single_value(const double latitude, const double longitude) {
    int latitude_int = border_to_int(latitude);
    int longitude_int = border_to_int(longitude);
    const std::string filename = get_filename(longitude_int, latitude_int);
    double result_part1[1];
    double *result_part2[1];
    result_part2[0] = result_part1;
    read_values(latitude_int, longitude_int, 1, 1, 0, 0, result_part2);
    return result_part2[0][0];
}

std::string SrtmReader::get_filename(const int west_border_int, const int north_border_int) {
    char file_to_open[256];
    get_filename(file_to_open, west_border_int, north_border_int);
    return std::string(file_to_open);
}
