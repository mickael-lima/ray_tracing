#include "../lib/material.hpp"
#include "../lib/utility.hpp"

// NOTE: Implementação do modelo de reflexão difusa de Lambertian. Consideramos,
// inicialmente, que o ponto onde o raio de luz bate e é refletido chama-se P. A
// partir disso, cria-se uma esfera tangente a reflexão no ponto P da
// superfície. O vetor que apontará para o centro da esfera tangente é dado por
// N + P (normal da sup + P). Ao fazer a soma vetorial de N + P com um vetor
// unitário aleatório, resultará em um novo vetor que está contido na esfera
// tangente, gerando raios não-uniformes e mais realistas.
bool Lambertian::scatter(const Ray &ray_in_sup, const HitRecord &rec, Vec3 &color_attenuation, Ray &scattered) const {

    // Implementação da difusão por meio da distribuição não uniforme de  Lambertian
    auto scatter_dir = rec.normal_sur_vector + Utility::random_unit_vec();

    // Caso ocorra de todos os módulos do vetor de reflexão tenderem a 0, a soma
    // acima pode acarretar bugs graças a limitação de represntação de ponto
    // flutuante, portanto é mais interessante considerar que os vetores de
    // reflexão e normal à superfície sejam iguais.
    if (scatter_dir.near_zero())
      scatter_dir = rec.normal_sur_vector;

    scattered = Ray(rec.point, scatter_dir);
    color_attenuation = m_color_albedo;
    return true;
}

bool Metal::scatter(const Ray &ray_in_sup, const HitRecord &rec, Vec3 &color_attenuation, Ray &scattered) const {
    Vec3 reflected_ray = Utility::reflect_vector(ray_in_sup.direction(), rec.normal_sur_vector);
    scattered = Ray(rec.point, reflected_ray);
    color_attenuation = m_color_albedo;
    return true;
}
