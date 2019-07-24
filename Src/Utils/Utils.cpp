#include <cmath>
#include "Utils.h"


#ifndef PI
#define PI 3.14159265358979323846
#endif


bool Utils::is_lesser(double a, double b) {
    return a-b < -2*EPSILON;
}

bool Utils::is_greater(double a, double b) {
    return a-b > 2*EPSILON;
}

bool Utils::equals(const double a, const double b) {
    return fabs(a - b) < EPSILON;
}

double Utils::floor2(double a) {
    double b = (int) a;
    if (!(!is_greater(b, a) && is_greater(b+1, a))) {
        ++b;
    }
    return b;
}

double Utils::ceil2(double a) {
    return floor2(a) + 1;
}


void Utils::change_bytes_order(uint16_t *var_ptr) {
    uint16_t tmp = *var_ptr;
    tmp <<= 8;
    (*var_ptr) >>= 8;
    (*var_ptr) |= tmp;
}


void Utils::swap_if_required(double *should_be_lower, double *should_be_bigger) {
    if ((*should_be_lower) > (*should_be_bigger)) {
        double tmp = (*should_be_lower);
        (*should_be_lower) = (*should_be_bigger);
        (*should_be_bigger) = tmp;
    }
}

size_t Utils::gcd(size_t a, size_t b) {
    do {
        if (b > a) {
            size_t tmp = a;
            a = b;
            b = tmp;
        }
        a -= b;
    } while (a != 0);
    return b;
}

double Utils::d2r(double degrees) {
    return degrees * PI / 180;
}

double Utils::r2d(double radians) {
    return radians * 180 / PI;
}


void Utils::shift(int from, int to, size_t *array) {
    for (int i = to; i > from; --i) {
        array[i] = array[i - 1];
    }
}

void Utils::verify_configs(Utils::config *config) {
    if (config->east_border < config->west_border) {
        config->east_border += 360.;
    }
}

std::pair<double, double> Utils::yy_transform(std::pair<double, double> coordinates) {
    std::pair<double, double> result = std::pair<double, double>();

    double x = - cos(coordinates.second * PI / 180.) * sin(coordinates.first * PI / 180.);
    double y = cos(coordinates.first * PI / 180.);
    double z = sin(coordinates.first * PI / 180.) * sin(coordinates.second * PI / 180.);

    result.first = acos(z) * 180. / PI;

    if (!Utils::is_greater(fabs(x), 1e-5)) {
        if (!Utils::is_greater(fabs(y), 1e-5)) {
            result.second = 0.;
        } else if (y > 0.) {
            result.second = 90.;
        } else if (y < 0.) {
            result.second = 270.;
        }
    } else {
        result.second = atan(y/x) * 180. / PI;
        if (x < 0.) {
            result.second += 180.;
        } else if (x > 0. && y< 0.) {
            result.second += 360.;
        }
    }

    return result;
}

