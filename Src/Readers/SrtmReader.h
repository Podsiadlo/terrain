#ifndef TERGEN_SRTMREADER_H
#define TERGEN_SRTMREADER_H


#include <cstdlib>
//#include "../Model/Map.h"
#include <string>
#include <unistd.h>

class SrtmReader {
private:
    static const int RESOLUTION = 3;

    static const unsigned short PIXEL_SIZE = 2;

    const std::string map_dir;

    void read_from_multiple_files(const double west_border, const double north_border, const double east_border,
                                  const double south_border, double **map_data);

    void read_values(int north_border_int, int west_border_int, size_t rows, size_t cols, int first_row,
                     int first_col, double **map_data);

    void
    read_from_file(int north_border_int, int west_border_int, char *filename, size_t rows, size_t cols, int first_row,
                   int first_col, double **map_data);

    void read_zeros(size_t rows, size_t cols, int first_row, int first_col, double **map_data);

    void skip_outliers(double *const *map_data, size_t length, size_t width);

    std::string get_filename(const int west_border_int, const int north_border_int);

    void get_filename(char *filename, int west_border_int, int north_border_int);

    int border_to_int(const double border) const;

public:
    SrtmReader(const std::string &mapDir) : map_dir(mapDir) {}

    static const int VALUES_IN_DEGREE = 60 * 60 / RESOLUTION;

//    Map *read_SRTM(const double west_border, const double north_border, const double east_border,
//                   const double south_border);

    double read_single_value(const double latitude, const double longitude);
};


#endif //TERGEN_SRTMREADER_H
