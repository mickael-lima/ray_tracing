#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits>
#include <cstdlib>

#include "vector3d.hpp"

namespace Utility {

    const double INFTY = std::numeric_limits<double>::infinity();
    const double PI = 3.1415926535897932385;

    inline double degrees_to_radian(double degrees) {
        return degrees * PI / 180.0;
    }

    // É possível obter números aleatórios utilizando <random> nativo de C++
    // mas para os fins de randomização de raios de luz, esse método serve
    double random_double();

    // NOTE: Cuidado com a chamada de random_double()
    double random_double(double min, double max);

    // Vetor aletório, essencial para simular reflexão em objetos difusos
    Vec3 random_vec();
    Vec3 random_vec(double min_val, double max_val);

    // Vetor aleatório dentro da esfera unitária (r = 1).
    Vec3 random_unit_vec();

    // Realiza um produto vetorial para verificar se o vetor aleatório está
    // apontando para dentro ou para fora do centro da esfera. Isso é feito
    // usando produto vetorial e verificando se o valor é maior que 0.
    //
    // n * d > 0.0 => cos() > 0 => ângulo entre 0 e 90 (vetor apontando na
    // direção centro -> fora da superfície) n * d < 0.0 => cos() < 0 => angulo
    // >90 (vetor apontando na direção fora da superficie -> dentro)
    Vec3 random_vec_on_hemisphere(const Vec3 &normal_vec);

    // Transforma um valor de cor linear em cor gamma aplicando um fator de
    // correção, fazendo com que a cor apareça mais natural ao ser exibida
    // em monitores. Isso é necessário, nesse contexto, para deixar a tran-
    // sição de cores mais suaves na hora de renderizar os pixels em .ppm;
    // Mais informações: https://docs.unity3d.com/Manual/LinearLighting.html
    double linear_to_gamma(double linear_color_value);
}// namespace

#endif // CONSTANTS_H_
