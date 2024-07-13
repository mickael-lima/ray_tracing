#include <iostream>
#include <fstream>
#include <filesystem>

#include "../lib/render.hpp"
#include "../lib/vector3d.hpp"
#include "../lib/ray.hpp"

// Essa função calcula se um determinado raio de luz P(t) = Q + td intercepta a esfera
// Matematicamente, partimos de (C - P)(C - P) = r², onde P = P(t) para sabermos
// se há interseção. Desenvolvendo essa expressão usando regras de produto escalar
// chega-se em t²(d * d) - 2t(d * (C - Q)) + (C - Q)(C - Q) - r² = 0, uma equação
// de segundo grau onde:
//
// - a = (d * d)
// - b = -2(d ((C - Q)))
// - c = (C - Q)(C - Q) - r²
//
// Se b² - 4ac < 0, então não intercepta, se b² - 4ac = 0, intercepta em apenas um ponto
// e por fim, se b² - 4ac > 0, intercepta em dois pontos.
bool Sphere::hit_sphere(const Ray& ray) {
    auto C_minus_Q = m_center - ray.origin();

    auto a = ray.direction() * ray.direction();
    auto b = -2.0 * (ray.direction() * C_minus_Q);
    auto c = (C_minus_Q * C_minus_Q) - m_radius*m_radius;

    auto discriminant = b*b - 4*a*c;

    return (discriminant >= 0);
}

// Essa função transforma um vetor de cor {R, G, B} em uma linha válida de PPM
// mais informações sobre o formato PPM pode ser encontrada nos comentários de
// Render::output_to_ppm()
void Render::write_color(std::ostream &out, const Vec3 &color) {
    auto r{color.x()};
    auto g{color.y()};
    auto b{color.z()};

    int rbyte{int(255.999 * r)};
    int gbyte{int(255.999 * g)};
    int bbyte{int(255.999 * b)};

    out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;
}

// Trataremos a cor no formato RGB, onde os valores de R, G e B são componentes de um vetor
void Render::output_to_ppm(const char *filename) {

    if (std::filesystem::exists(filename))
        std::clog << "[AVISO] arquivo " << filename << " existe, seu conteúdo será sobreescrito" << std::endl;

    // Objeto para renderizar
    Sphere obj_sphere(Vec3(0,0,-1), 0.5);

    // Arquivo para escrever a renderização em formado .PPM
    std::ofstream output_file(filename, std::ofstream::out | std::ofstream::trunc);

    // https://en.wikipedia.org/wiki/Netpbm#PPM_example
    output_file << "P3" << std::endl;
    output_file << m_img_width << ' ' << m_img_height << std::endl;
    output_file << "255" << std::endl;

    // Qualquer coisa abaixo de "255" é considerado como conteúdo
    // A ordem de informação é RGBYWB (red/green/blue/yellow/white/black)
    for(auto j = 0; j < m_img_height; ++j) {
        std::clog << "\nLinhas verticias restantes: " << (m_img_height - j) << ' ' << std::flush;

        for(auto i = 0; i < m_img_width; ++i) {

            auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_i) + (j * m_pixel_delta_j);
            auto ray_direction = pixel_center - m_camera_center;

            Ray r(m_camera_center, ray_direction);
            Vec3 pixel_color{};

            if(obj_sphere.hit_sphere(r)) {
                pixel_color = Vec3{0,1,0};
            } else {
                pixel_color = r.ray_color();
            }

            write_color(output_file, pixel_color);
        }
    }

    output_file.close();
    std::clog << std::endl << "Concluído" << std::endl;
}
