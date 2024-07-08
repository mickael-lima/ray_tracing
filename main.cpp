#include <iostream>
#include "lib/render.hpp"

auto main(int argc, char *argv[]) -> int {

    Render render_obj{};
    render_obj.output_to_ppm();

    return 0;
}
