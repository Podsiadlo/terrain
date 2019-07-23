#ifndef TERGEN_PROCESSOR_H
#define TERGEN_PROCESSOR_H

#include <utility>
#include <cmath>
#include "../Model/Map.h"
#include "../Utils/Cuboid.h"

class Processor {
public:

    Processor(const std::pair<double, double> &xRange, const std::pair<double, double> &yRange,
              const std::pair<double, double> &zRange, int indexX, int indexY, int indexZ, int xResolution,
              int yResolution, int zResolution, Map *map) : x_range(xRange), y_range(yRange), z_range(zRange), index_x(indexX),
                                                           index_y(indexY), index_z(indexZ), x_resolution(xResolution),
                                                           y_resolution(yResolution), z_resolution(zResolution),
                                                           map(map) {
        x_step = fabs(xRange.first - xRange.second) / (xResolution - 1);
        y_step = fabs(yRange.first - yRange.second) / (yResolution - 1);
        z_step = fabs(zRange.first - zRange.second) / (zResolution - 1);
    }

    void calculate(std::string fileBaseName);

private:
    std::pair<double, double> x_range;
    std::pair<double, double> y_range;
    std::pair<double, double> z_range;
    int index_x;
    int index_y;
    int index_z;
    int x_resolution;
    int y_resolution;
    int z_resolution;
    double x_step;
    double y_step;
    double z_step;
    Map *map;

    void fill_cuboid(Cuboid<bool> &cuboid) const;

    void print_cuboid(Cuboid<bool> cuboid, std::string fileBaseName);

    std::string get_filename(std::string fileBaseName);
};


#endif //TERGEN_PROCESSOR_H
