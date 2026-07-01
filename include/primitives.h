#pragma once


#include "mesh.h"


namespace primitives {
    Mesh make_cube(float size = 1.0f);
    Mesh make_sphere(float rho = 1.0f, uint32_t rings = 16, uint32_t sectors = 32);
}

