#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "objects.hpp"
#include "ray.hpp"

class Material {
    public:
        virtual ~Material() = default;

        // Retorna se o raio de luz incidido na superfície é dividido em vários raios menores de direção aleatória
        // ao ser refletido (true) ou não (false)
        virtual bool scatter(const Ray &ray_in_sup, const HitRecord &rec, Vec3& color_attenuation, Ray &scattered) const {
            return false;
        }
};

// Implementação de material difuso. Esses objetos podem desviar a luz e absorver uma parte dela, sempre desviar ou
// sempre absorver, para essa implementação escolheu-se sempre desviar para simplicidade.
class Lambertian : public Material {
    public:
        Lambertian(const Vec3& color_albedo) :
            m_color_albedo{color_albedo} {}

        bool scatter(const Ray &ray_in_sup, const HitRecord &rec, Vec3& color_attenuation, Ray &scattered) const override;

    private:
        // Albedo é um termo em latim que significa "intensidade da cor branca".
        Vec3 m_color_albedo;
};

class Metal : public Material {
    public:
        Metal(const Vec3 &color_albedo) :
            m_color_albedo{color_albedo} {}

        bool scatter(const Ray &ray_in_sup, const HitRecord &rec, Vec3& color_attenuation, Ray &scattered) const override;

    private:
        Vec3 m_color_albedo;
};

#endif // _MATERIAL_HPP_
