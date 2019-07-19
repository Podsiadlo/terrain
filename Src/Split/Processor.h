#ifndef TERGEN_PROCESSOR_H
#define TERGEN_PROCESSOR_H

#include <utility>

class Processor {
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

public:
    Processor(std::pair<double, double>, std::pair<double, double>, std::pair<double, double>, int, int, int, int, int, int);
    void calculate(char*);
};


#endif //TERGEN_PROCESSOR_H
