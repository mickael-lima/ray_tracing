#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits>
#include <cstdlib>

const double INFTY = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radian(double degrees) {return degrees * PI / 180.0;}

// É possível obter números aleatórios utilizando <random> nativo de C++
// mas para os fins de randomização de raios de luz, esse método serve
inline double random_double() {
    // Sempre resultará em um número decimal
    return std::rand() / (RAND_MAX + 1.0);
}

// NOTE: Cuidado com a chamada de random_double()
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif // CONSTANTS_H_
