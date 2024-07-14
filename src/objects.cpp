#include "../lib/ray.hpp"
#include "../lib/objects.hpp"

// Pela álgebra linear, o produto escalar entre dois vetores u, v pode ser computado usando
// a expressão: u * v = ||u|| ||v|| * cos(a), onde a é o ângulo entre u e v. considere v como
// unitário, então u * v = ||u|| * cos(a). Note que cos(a) = u / ||u||, que inevitavelmente
// será unitário também. Se cos(a) < 0, então os vetores está em sentidos opostos, se cos(a) > 0
// então os vetores estarão no mesmo sentido. NOTE: é por isso que outward_normal tem que ser unitário.
void HitRecord::set_face_normal(const Ray &r, const Vec3 &outward_normal) {

    // Se for menor que 0, então o vetor direcional do raio de luz e o vetor da superfície são
    // opostos, logo estamos lidando com a superfície externa. Caso contrário, estamos lidando
    // com uma superfície interna (a iluminação está "saíndo de dentro pra fora")
    front_face = (r.direction() * outward_normal) < 0;

    // Se o raio de luz estiver interagindo com a superfície externa, então o vetor normal é o
    // outward_normal. Caso contrário, entõ é o vetor oposto ao outward_normal.
    normal_sur_vector = front_face ? outward_normal : -outward_normal;

}

// Essa função calcula se um determinado raio de luz P(t) = Q + td intercepta a esfera
// Matematicamente, partimos de (C - P)(C - P) = r², onde P = P(t) para sabermos
// se há interseção. Desenvolvendo essa expressão usando regras de produto escalar
// chega-se em t²(d * d) - 2t(d * (C - Q)) + (C - Q)(C - Q) - r² = 0, uma equação
// de segundo grau onde:
//
// - a = (d * d) => (o produto vetorial entre o vetor d e ele mesmo equivale a norma ao quadrado)
// - b = -2(d * (C - Q))
// - c = (C - Q)(C - Q) - r²
//
// A resolução da equação de segundo grau acima retorna para qual t o raio de luz intercepta
// a esfera
//
// NOTE (OTIMIZAÇÃO): Seja b = -2h, então t = ( -b +/- sqrt(b² - 4ac) / 2a ) => (2h +/- sqrt((-2h)² - 4ac) / 2a)
// portanto (2h +/- sqrt(4h^2 - 4ac) / 2a) => (h +/- sqrt(h² - ac) / a). Com essas operações
// de substituição, então b = -2(d * (C - Q)) = h +/- sqrt(h² - ac) / a => h = -(b/2) = d * (C - Q).
//
// Essa simplificação forçada diminui o número de operações matemáticas necessárias para achar t.
bool Sphere::hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord &h_rec ) const {
    auto C_minus_Q = m_center - ray.origin();

    auto a = (ray.direction()).length() * (ray.direction().length());
    auto h = ray.direction() * C_minus_Q;
    auto c = (C_minus_Q * C_minus_Q) - m_radius*m_radius;

    auto discriminant = h*h - a*c;

    if (discriminant < 0)
        return false;

    auto t1 = (h - std::sqrt(discriminant)) / a;
    auto t2 = (h + std::sqrt(discriminant)) / a;

    auto valid_t = t1;

    // Se as raízes, que são valores para o t do raio, não estiverem entre ray_tmin e ray_tmax
    // então o raio não contabilizou
    if(t1 <= ray_tmin || t1 >= ray_tmax) {
        valid_t = t2; // consideramos aqui que t2 seja um possível valor válido para t

        if(t2 <= ray_tmin || t2 >= ray_tmax) {
            return false;
        }
    }

    h_rec.t = valid_t;
    h_rec.point = ray.at(valid_t);

    // O vetor normal da superfície esférica no ponto sempre será (ponto - centro) / raio
    // O vetor normal final leva em conta o sentido do raio de luz e sua interação com o objeto
    Vec3 outward_normal = (h_rec.point - m_center) / m_radius;
    h_rec.set_face_normal(ray, outward_normal);

    return true;
}

//
bool HittableList::hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord &h_rec) const {
    HitRecord temp_h_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_tmax;

    for (const auto& object : objects) {
        if(object->hit(r, ray_tmin, closest_so_far, temp_h_rec)) {
            hit_anything = true;
            closest_so_far = temp_h_rec.t;
            h_rec = temp_h_rec;
        }
    }

    return hit_anything;
}
