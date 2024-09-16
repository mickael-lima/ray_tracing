#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <memory>
#include <vector>

#include "vector3d.hpp"
#include "ray.hpp"
#include "interval.hpp"

class Material; // NOTE: Evita problemas de dependência ciclica entre as classes Material e HitRecord

// Esse header contém os objetos que queremos renderizar. Todos serão deriváveis de uma classe
// puramente virtul que servirar como base, chamada de "Hittable" (ou seja, tudo que pode ser
// "tocado" pela luz)

// Essa estrutura armazena informações de quando um raio de luz toca em um ponto, são elas:
// 1. O ponto em si
// 2. O vetor normal ao ponto
// 3. O valor do parâmetro t da equação da reta do raio de luz
class HitRecord {
    public:
        Point3 point;
        Vec3 normal_sur_vector;
        double t;
        bool is_front_face;
        std::shared_ptr<Material> obj_material;

        // Por convenção, todos os vetores normais à superfície do objeto devem
        // apontar para fora (no mesmo sentido do vetor centro->ponto na superfície)
        // NOTE: o vetor outward_normal deve ser obrigatoriamente unitário para a
        // função abaixo fazer sentido.
        void set_face_normal(const Ray& r, const Vec3& outward_normal);
};

class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray &r, Interval acceptable_t_interval, HitRecord &h_rec) const = 0;
};

class Sphere : public Hittable {
    public:
        // NOTE: raio não pode ser negativo
        Sphere(const Vec3 &center, double radius, std::shared_ptr<Material> material)
                : m_center(center), m_radius(fmax(0, radius)), m_material{material} {}

        Vec3 center() { return m_center; };
        double radius() { return m_radius; };

        // O raio contará como "tocado" se o t obtido estiver contido no intervalo aberto (ray_tmin, ray_tmax)
        // isso é: ray_tmin < t < ray_tmax
        bool hit(const Ray& ray, Interval acceptable_t_interval, HitRecord &h_rec) const override;

    private:
        Vec3 m_center{};
        double m_radius;
        std::shared_ptr<Material> m_material;
};

// Uma lista/coleção/agrupamento de todos os objetos que são "tocáveis". Seria uma espécie de "mundo" onde
// todas as coisas renderizáveis abitam.
class HittableList : public Hittable {
    public:
        std::vector<std::shared_ptr<Hittable>> objects;

        HittableList() {}
        explicit HittableList(std::shared_ptr<Hittable> object) { add_to_obj_list(object);  }

        void clear() { objects.clear(); };
        void add_to_obj_list(std::shared_ptr<Hittable> object) { objects.push_back(object); };

        bool hit(const Ray& r, Interval acceptable_t_interval, HitRecord &rec) const override;
};

#endif // OBJECTS_H_
