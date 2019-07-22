#include <fstream>
#include "Processor.h"
#include "../Utils/Cuboid.h"

void Processor::calculate(char *fileBaseName) {
    Cuboid<bool> cuboid(x_resolution, y_resolution, z_resolution);
    fill_cuboid(cuboid);
    print_cuboid(cuboid, fileBaseName);
}

void Processor::fill_cuboid(Cuboid<bool> &cuboid) const {
    for (int i = 0; i < x_resolution; ++i) {
        for (int j = 0; j < y_resolution; ++j) {
            double height = map.getValue(x_range.first + i * x_step, y_range.first + j * y_step);
            for (int k = 0; k < z_resolution; ++k) {
                cuboid.set(i, j, k, z_range.first + k * z_step <= height);
            }
        }
    }
}

void Processor::print_cuboid(Cuboid<bool> cuboid, char *fileBaseName) {
    std::string filename = get_filename(fileBaseName);
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::binary);
    for (const auto &e : cuboid.getVector()) {
        file << (char) e;
    }
    file.close();
}

std::string Processor::get_filename(char *fileBaseName) {
    return std::string(fileBaseName) + "_" + std::to_string(index_x) + "_" + std::to_string(index_y) + "_" +
           std::to_string(index_z) + ".data";
}

