#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits>

const double INFTY = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radian(double degrees) {
    return degrees * PI / 180.0;
}



#endif // CONSTANTS_H_
