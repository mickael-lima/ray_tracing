#ifndef RAY_H_
#define RAY_H_

#include "vector3d.hpp"

// TODO: criar uma classe principal Point3 e fazer com que Vec3 seja classe filha desta
using Point3 = Vec3;

// Um raio de luz pode ser descrito matematicamente como uma reta P(t) = Bt + A
class Ray {
    private:
        Point3 m_ray_origin;
        Vec3 m_ray_direction;

    public:
        Ray(); // Point3 inicializará todos os vetores com (0,0,0) se nenhum argumento for dado, portanto isso é seguro
        Ray(const Point3 &origin_point, const Vec3 &ray_direction)
            : m_ray_origin{origin_point}, m_ray_direction{ray_direction} {}

        const Point3 &origin() const {return m_ray_origin;}
        const Point3 &direction() const {return m_ray_direction;}

        // Tratar como função vetorial da reta é o caminho mais simples
        // at(t) = (x_d, y_d, z_d) + t(x_0, y_0, z_0)
        Point3 at(double t) const {return (m_ray_direction * t) + m_ray_origin;};

        Vec3 ray_color() {
            Vec3 unit_direction = vetor_unit(m_ray_direction);
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*Vec3(1.0, 1.0, 1.0) + a*Vec3(0.5, 0.7, 1.0);
        }
};

#endif // RAY_H_
