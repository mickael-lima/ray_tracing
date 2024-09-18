#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <thread>

#include "../lib/render.hpp"
#include "../lib/vector3d.hpp"
#include "../lib/objects.hpp"
#include "../lib/ray.hpp"
#include "../lib/utility.hpp"
#include "../lib/material.hpp"

// Essa função transforma um vetor de cor {R, G, B} em uma linha válida de PPM
// mais informações sobre o formato PPM pode ser encontrada nos comentários de
// Render::output_to_ppm()
void Render::write_color(std::ostream &out, const Vec3 &color) {

    static const Interval intensity_level(0.000, 0.999);

    auto r{color.x()};
    auto g{color.y()};
    auto b{color.z()};

    // Aplica correção de cor pela conversão do espaço linear para espaço gamma.
    // Nessa implementação, utilizou-se o espaço "gamma 2", que consiste em elevar
    // o expoente do valor linear pelo inverso de 2 (tirar raíz quadrada).
    r = Utility::linear_to_gamma(r);
    g = Utility::linear_to_gamma(g);
    b = Utility::linear_to_gamma(b);

    int rbyte{int(256 * intensity_level.restrict_to_interval(r))};
    int gbyte{int(256 * intensity_level.restrict_to_interval(g))};
    int bbyte{int(256 * intensity_level.restrict_to_interval(b))};

    out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;
}

Vec3 Render::ray_color(const Ray &r, const Hittable &world, int recursive_depth) {

    // A cor (0,0,0) serve para representar ausencia de luz
    if(recursive_depth <= 0)
        return Vec3{0,0,0};

    HitRecord rec;

    // NOTE: Se o raio atingir o objeto, retorne cinza (intermediário  entre [0,0,0] e [1,1,1]), se não retorne
    // o valor original para cor do raio.
    //
    // Utilizar um intervalo (0, +INFTY) desencadeia um problema causado pelas limitações de pontos flutuantes. Se
    // considerarmos t = 0.0000001, a tendência é que  t  seja arredondado para 0, de modo que
    //
    // 1. t = 0.00000001
    // 2. t = 0.00000000001
    // 3. t = 0.000000000000001
    // etc...
    //
    // Sejam considerado o mesmo raio de luz. Pare resolver esse bug, consideraremos como ponto inicial um intervalo
    // um pouco maior do que 0.
    if(world.hit(r, Interval(0.001, +Utility::INFTY), rec)) {
        Ray scattered;
        Vec3 color_attenuation;

        if(rec.obj_material->scatter(r, rec, color_attenuation, scattered)) {
            auto color = ray_color(scattered, world, recursive_depth - 1);
            return Utility::product_component(color_attenuation, color);
        }

        return Vec3{0,0,0};
    }

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
    return Vec3((Utility::random_double() - 0.5), (Utility::random_double() - 0.5), 0);
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

void Render::render_quadrant(std::ostream &output_file, int quadrant, HittableList &world) {

    int start_i = 0;
    int start_j = 0;
    int end_i = 0;
    int end_j = 0;

    switch(quadrant) {
        case 1: {
            start_i = 0;
            start_j = 0;
            end_i = m_img_width;
            end_j = m_img_height / 4;
            break;
        }

        case 2: {
            start_i = 0;
            start_j = m_img_height / 4;
            end_i = m_img_width;
            end_j = m_img_height / 3;
            break;
        }

        case 3: {
            start_i = 0;
            start_j = m_img_height / 3;
            end_i = m_img_width;
            end_j = m_img_height / 2;
            break;
        }

        case 4: {
            start_i = 0;
            start_j = m_img_height / 2;
            end_i = m_img_width;
            end_j = m_img_height;
            break;
        }

        default: {
            start_i = 0;
            start_j = 0;
            end_i = m_img_width;
            end_j = m_img_height;
            break;
        }
    }

    // Qualquer coisa abaixo de "255" é considerado como conteúdo
    // A ordem de informação é RGBYWB (red/green/blue/yellow/white/black)
    for (auto j = start_j; j < end_j; ++j) {
      std::clog << "\nLinhas verticias restantes: " << (m_img_height - j) << ' ' << std::flush;

      for (auto i = start_i; i < end_i; ++i) {
        Vec3 pixel_color{0, 0, 0};

        for (auto sample = 0; sample < m_ray_sample_per_pixel; ++sample) {
          Ray r = get_ray(i, j);
          pixel_color += ray_color(r, world, m_max_recursive_depth);
        }

        write_color(output_file, m_ray_sample_scale * pixel_color);
      }
    }
}

void Render::merge_renderized_cache_files(std::ostream &final_file, std::vector<const char*> cache_filenames)  {
    std::vector<std::ifstream> cache_content;

    for(auto &cache_filename : cache_filenames)
      cache_content.push_back(std::ifstream(cache_filename, std::ios_base::binary));

    for(auto &content : cache_content) {
      final_file << content.rdbuf();
      content.close();
    }
}

// Trataremos a cor no formato RGB, onde os valores de R, G e B são componentes de um vetor
void Render::output_to_ppm(const char *filename) {

    if (std::filesystem::exists(filename))
        std::clog << "[AVISO] arquivo " << filename << " existe, seu conteúdo será sobreescrito" << std::endl;

    // Lista de objetos que será renderizado
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Vec3{0.8, 0.8, 0.0});
    auto material_center = std::make_shared<Lambertian>(Vec3{0.1, 0.2, 0.5});
    auto material_left = std::make_shared<Metal>(Vec3{0.8, 0.8, 0.8});
    auto material_right = std::make_shared<Metal>(Vec3{0.8, 0.6, 0.2});

    world.add_to_obj_list(std::make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add_to_obj_list(std::make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add_to_obj_list(std::make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add_to_obj_list(std::make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    std::vector<const char*> cache_filenames = {"render_cache_1", "render_cache_2", "render_cache_3", "render_cache_4"};
    std::vector<std::ofstream> cache_files;

    // Arquivos soltos para renderizar cada pedaço da cena, necessário para
    // renderizar paralelamente.
    for (auto &cache_filename : cache_filenames)
      cache_files.push_back(std::ofstream(cache_filename, std::ofstream::out | std::ofstream::trunc | std::ios_base::binary));

    // Arquivo final, onde de fato o PPM será gerado
    std::ofstream output_file(filename, std::ofstream::out | std::ofstream::trunc | std::ios_base::binary);

    // https://en.wikipedia.org/wiki/Netpbm#PPM_example
    output_file << "P3" << std::endl;
    output_file << m_img_width << ' ' << m_img_height << std::endl;
    output_file << "255" << std::endl;

    // Renderiza a cena em 4 arquivos diferentes

    std::thread render1(&Render::render_quadrant, this, std::ref(cache_files.at(0)), 1, std::ref(world));
    std::thread render2(&Render::render_quadrant, this, std::ref(cache_files.at(1)), 2, std::ref(world));
    std::thread render3(&Render::render_quadrant, this, std::ref(cache_files.at(2)), 3, std::ref(world));
    std::thread render4(&Render::render_quadrant, this, std::ref(cache_files.at(3)), 4, std::ref(world));

    render1.join();
    render2.join();
    render3.join();
    render4.join();

    // Junta-se as cenas em 1 arquivo final, que será o .ppm
    merge_renderized_cache_files(output_file, cache_filenames);

    // Necessário fechar os streams para não ocorrer nenhum tipo de vazamento de memória
    for(auto &cache_file : cache_files)
        cache_file.close();

    output_file.close();
    std::clog << std::endl << "Concluído" << std::endl;
}
