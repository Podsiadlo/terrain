#ifndef TERGEN_CUBOID_H
#define TERGEN_CUBOID_H

#include <vector>

template<typename T>
class Cuboid {
private:
    int x_size;
    int y_size;
    int z_size;
    std::vector<T> points;

public:
    Cuboid(int xSize, int ySize, int zSize) : x_size(xSize), y_size(ySize), z_size(zSize),
                                              points(std::vector<T>(xSize * ySize * zSize)) {}

    void set(int x, int y, int z, T value);

    T get(int x, int y, int z);

    std::vector<T> getVector();

private:
    int getIndex(int x, int y, int z);
};


#endif //TERGEN_CUBOID_H
