#include <iostream>

#include "../lib/render.hpp"

void Render::output_to_ppm() {

    // https://en.wikipedia.org/wiki/Netpbm#PPM_example
    std::cout << "P3" << std::endl;
    std::cout << img_width << ' ' << img_height << std::endl;
    std::cout << "255" << std::endl;

    // Qualquer coisa abaixo de "255" é considerado como conteúdo
    // A ordem de informação é RGBYWB (red/green/blue/yellow/white/black)

    for(auto j = 0; j < img_height; ++j) {
        std::clog << "\nLinhas verticias restantes: " << (img_height - j) << ' ' << std::flush;
        std::cout << std::endl;

        for(auto i = 0; i < img_width; ++i) {

            auto r = double(i) / (img_width - 1);
            auto g = double(j) / (img_height - 1);
            auto b{0.0};

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << std::endl << "Concluído" << std::endl;
}
