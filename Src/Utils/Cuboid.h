#ifndef TERGEN_CUBOID_H
#define TERGEN_CUBOID_H

#include <vector>

template<typename T>
class Cuboid {
private:
    size_t x_size;
    size_t y_size;
    size_t z_size;
    std::vector<T> points;

public:
    Cuboid(size_t xSize, size_t ySize, size_t zSize) : x_size(xSize), y_size(ySize), z_size(zSize),
                                              points(std::vector<T>(xSize * ySize * zSize)) {}

    void set(size_t x, size_t y, size_t z, T value) {
        points[getIndex(x, y, z)] = value;
    }

    T get(size_t x, size_t y, size_t z) {
        return points[getIndex(x, y, z)];
    }

    std::vector<T> getVector() {
        return points;
    }

private:
    size_t getIndex(size_t x, size_t y, size_t z) {
        return z * x_size * y_size + y * x_size + x;
    }
};


#endif //TERGEN_CUBOID_H
