#include "Processor.h"

Processor::Processor(std::pair<double, double> x_range, std::pair<double, double> y_range,
                     std::pair<double, double> z_range, int x_resolution, int y_resolution, int z_resolution,
                     int index_x, int index_y, int index_z) {

    this->x_range = x_range;
    this->y_range = y_range;
    this->z_range = z_range;
    this->x_resolution = x_resolution;
    this->y_resolution = y_resolution;
    this->z_resolution = z_resolution;
    this->index_x = index_x;
    this->index_y = index_y;
    this->index_z = index_z;
}

void Processor::calculate(char *) {

}

