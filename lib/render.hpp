#ifndef _RENDER_H_
#define _RENDER_H_

#include <iostream>
#include "vector3d.hpp"
#include "ray.hpp"

class Render {
    public:
        explicit Render(int img_width = 400, double viewport_height = 2.0)
            : m_img_width{img_width},
              m_viewport_height{viewport_height},

              // Computa-se a altura em função da largura e do aspect ratio
              // 16 / 9 = largura / altura => altura = (9 * largura)/16 = altura, ou altura = largura / (16/9)
              // Se a altura for menor que 1 na conversão, considere h = 1 para evitar complicações
              m_img_height{ (int(m_img_width / aspect_ratio) < 1) ? 1 : (int(m_img_width / aspect_ratio)) },

              // Não é recomendável usar o valor puro do aspect ratio para esse cálculo porque
              // o aspect ratio real pode diferir de 16/9 por causa do arredondamentos feito com int().
              // NOTE: viewport_width / viewport_height = 16/9 => viewport_width = viewport_height * 16/9
              // de modo que 16/9 é aprox imd_width / img_height
              m_viewport_width{m_viewport_height * (double(m_img_width) / m_img_height)} {}

        void output_to_ppm();
        void write_color(std::ostream &out, const Vec3 &color);

    private:
        double aspect_ratio = 16.0 / 9.0;

        // O viewport refere-se a um retângulo inserido no espaço. Funciona como uma "janela" 2D para o 3D
        // m_img_* referem-se as dimensões da imagem
        int m_img_width;
        double m_viewport_height;

        int m_img_height;
        double m_viewport_width;

        // Camera, o ponto no espaço no qual todos os vetores "raio de luz" terão como origem
        // O vetor com origem no centro da câmera e final no centro do viewport será ortogonal.
        Vec3 m_camera_center; // A câmera ficará no ponto (0, 0, 0) por enquanto
        double m_focal_length{1.0}; // Distância entre o viewport e a câmera

        // As convenções para renderização da imagem usadas nesse programa são:
        // 1. O ponto de origem (0,0,0) está no canto superior esquerdo
        // 2. A imagem renderiza da esquerda pra direita, depois de cima para baixo
        //
        // Usaremos dois vetores viewport_i e viewport_j que tem origem (0,0,0) e apontam
        // para (1, 0, 0) e (0, -1, 0), respectivamente. No entanto, esses vetores são
        // esticados em função da altura e largura do viewport.

        Vec3 m_viewport_i{m_viewport_width}; // (1, 0 0)
        Vec3 m_viewport_j{0, -m_viewport_height}; // (1, 0 0)

        // Seja delta i a distância horizontal entre 2 pixels, então delta i * m_img_width = viewport_i
        // Logo, a distância entre 2 pixels na horizontal é exprimida por delta i = viewport_i / m_img_width
        // Analogo para a distância vertical.
        Vec3 m_pixel_delta_i = m_viewport_i / m_img_width;
        Vec3 m_pixel_delta_j = m_viewport_j / m_img_height;

        // O ponto da extrema superior esquerda do viewport, está um pouco acima de (0,0,0).
        // NOTE: porque isso funciona?
        // https://raytracing.github.io/images/fig-1.04-pixel-grid.jpg
        Vec3 m_viewport_upper_left{m_camera_center - Vec3(0, 0, m_focal_length) - m_viewport_i/2 - m_viewport_j/2};
        Vec3 m_pixel00_loc{m_viewport_upper_left + 0.5 * (m_pixel_delta_i + m_pixel_delta_j)};
};

class Sphere {
    public:
        Sphere(const Vec3 &center, double radius)
            : m_center(center), m_radius(radius) {};

        bool hit_sphere(const Ray& ray);

    private:
        Vec3 m_center{};
        double m_radius;
};

#endif
