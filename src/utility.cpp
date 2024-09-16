#include "../lib/utility.hpp"
#include "../lib/vector3d.hpp"

double Utility::random_double() {
    // Sempre resultará em um número decimal
    return std::rand() / (RAND_MAX + 1.0);
}

double Utility::random_double(double min, double max) {
    return min + (max - min) * Utility::random_double();
}

Vec3 Utility::random_vec(double min_val, double max_val) {
    return Vec3{Utility::random_double(min_val, max_val),
                Utility::random_double(min_val, max_val),
                Utility::random_double(min_val, max_val)};
}
    // Vetor aletório, essencial para simular reflexão em objetos difusos
Vec3 Utility::random_vec() {
    return Vec3{Utility::random_double(), Utility::random_double(), Utility::random_double()};
}

Vec3 Utility::random_unit_vec() {

    // NOTE: Utiliza-se o método da tentativa e descarte para achar um vetor que
    // esteja contido na esfera se for aceito, então retorna o vetor unitário com
    // a ponta exatamente em algum ponto da superfície
    while (true) {
      Vec3 random_vec = Utility::random_vec(-1, 1);
      auto random_vec_length = random_vec.squared_length();

      // NOTE: É preciso verificar se o tamanho d^2 é maior que esse expoente
      // pequeno para evitar problemas em relação a aproximação de float. Caso
      // contrário, a divisão random_vec / (random_vec_length)^1/2 resultará em
      // componentes tendendo ao infinito pelo denominador se aproximando de 0.
      if (random_vec_length <= 1 && 1e-160 < random_vec_length)
          return random_vec / std::sqrt(random_vec_length);
    }
}

Vec3 Utility::random_vec_on_hemisphere(const Vec3& normal_vec) {
    Vec3 random_on_sphere_vec = Utility::random_unit_vec();

    if (random_on_sphere_vec * normal_vec > 0.0)
        return random_on_sphere_vec;
    else
        return -random_on_sphere_vec;
}

double Utility::linear_to_gamma(double linear_color_value) {
    if(linear_color_value > 0)
      return std::sqrt(linear_color_value);

    return 0;
}

Vec3 Utility::product_component(const Vec3 &vector_a, const Vec3 &vector_b) {
  return Vec3{vector_a.x() * vector_b.x(),
              vector_a.y() * vector_b.y(),
              vector_a.z() * vector_b.z()
  };
}

// Considere o modelo de reflexão em materiais metálicos: https://raytracing.github.io/images/fig-1.15-reflection.jpg
// Pela operação com vetores, sabe-se que V + 2B resulta em um vetor R que tem a mesma direção e sentido do vetor
// procurado na imagem. Para determinar o valor de B, projeta-se o vetor V em B. Pela formula de projeção de vetor e
// considerando o fato que N sempre será unitário, é necessário apenas o produto escalar (V * N)*N para determinar B.
// A fórmula fica V + 2(V * N). Dado que consideramos B sendo refletido para fora (veja a imagem), é necessário inver-
// ter seu caminho multiplicando-o por -1. A fórmula final para essa reflexão simples é R = V - 2(N * V)*N
Vec3 Utility::reflect_vector(const Vec3 &vector, const Vec3 &normal) {
   return vector - (2 * (vector * normal) * normal);
}
