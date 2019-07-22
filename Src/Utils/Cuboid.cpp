#include "Cuboid.h"

template<typename T>
T Cuboid<T>::get(int x, int y, int z) {
    return points[getIndex(x, y, z)];
}

template<typename T>
void Cuboid<T>::set(int x, int y, int z, T value) {
    points[getIndex(x, y, z)] = value;
}

template<typename T>
int Cuboid<T>::getIndex(int x, int y, int z) {
    return z * x_size * y_size + y * x_size + x;
}

template<typename T>
std::vector<T> Cuboid<T>::getVector() {
    return points;
}
