#include <iostream>
#include <cstring>
#include "lib/render.hpp"

auto main(int argc, char *argv[]) -> int {

    Render ray_tracing_instance{};

    if (argc == 3 && strcmp(argv[1], "--output") == 0)
        ray_tracing_instance.output_to_ppm(argv[2]);
    else {
        std::cerr << "[ERRO] Uso: ./" << argv[0] << " --output arquivo.ppm" << std::endl;
        return -1;
    }

    return 0;
}
