#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>

#include "../lib/render.hpp"
#include "../lib/vector3d.hpp"
#include "../lib/objects.hpp"
#include "../lib/ray.hpp"
#include "../lib/utility.hpp"

// Essa função transforma um vetor de cor {R, G, B} em uma linha válida de PPM
// mais informações sobre o formato PPM pode ser encontrada nos comentários de
// Render::output_to_ppm()
void Render::write_color(std::ostream &out, const Vec3 &color) {

    static const Interval intensity_level(0.000, 0.999);

    auto r{color.x()};
    auto g{color.y()};
    auto b{color.z()};

    int rbyte{int(256 * intensity_level.restrict_to_interval(r))};
    int gbyte{int(256 * intensity_level.restrict_to_interval(g))};
    int bbyte{int(256 * intensity_level.restrict_to_interval(b))};

    out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;
}

Vec3 Render::ray_color(const Ray &r, const Hittable &world) {
    HitRecord rec;

    // Consideramos t_max igual infinito para gerar a imagem especifica
    if(world.hit(r, Interval(0.0, +INFTY), rec))
        return 0.5 * (rec.normal_sur_vector + Vec3{1, 1, 1});

    Vec3 unit_direction = r.direction().unit();
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*Vec3(1.0, 1.0, 1.0) + a*Vec3(0.5, 0.7, 1.0);
}

// NOTE: Função auxiliar para emular um pixel em forma quadrado unitário para pegar algum
// ponto aleatório dentro desse quadrado
Vec3 random_vec_from_unit_square() {

    // O quadrado unitário está na região x = {-0.5, 0.5}, y = {-0.5, 0.5} e random_double()
    // retornará sempre um número menor que 1, portanto o vetor abaixo está nos limites do
    // quadrado unitário estabelecido
    return Vec3((random_double() - 0.5), (random_double() - 0.5), 0);
}

//  Joga um raio com origem no centro da câmera até um ponto escolhido aleatoriamente na coordenada (i, j)
//  Isso faz com que haja uma maior naturalidade nos raios de luz em direção ao objeto, já que na vida real
//  a luz não bate no objeto sempre na mesma localização, essencial para erar o efeito de antialiasing.
Ray Render::get_ray(int i, int j) const {
    auto offset = random_vec_from_unit_square();
    auto pixel_sample = m_pixel00_loc
                          + ((i + offset.x()) * m_pixel_delta_i)
                          + ((j + offset.y()) * m_pixel_delta_j);

    auto ray_origin = m_center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray{ray_origin, ray_direction};

}

// Trataremos a cor no formato RGB, onde os valores de R, G e B são componentes de um vetor
void Render::output_to_ppm(const char *filename) {

    if (std::filesystem::exists(filename))
        std::clog << "[AVISO] arquivo " << filename << " existe, seu conteúdo será sobreescrito" << std::endl;

    // Lista de objetos que será renderizado
    HittableList world;

    world.add_to_obj_list(std::make_shared<Sphere>(Point3(0, 0, -1.0), 0.5));
    world.add_to_obj_list(std::make_shared<Sphere>(Point3(0, -100.5, -1.0), 100));

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
            Vec3 pixel_color{0,0,0};

            for(auto sample = 0; sample < m_ray_sample_per_pixel; ++sample) {
                Ray r = get_ray(i, j);
                pixel_color += ray_color(r, world);
            }

            write_color(output_file, m_ray_sample_scale * pixel_color);
        }
    }

    output_file.close();
    std::clog << std::endl << "Concluído" << std::endl;
}
