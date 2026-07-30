#pragma once

#include <vector>

#include "camera.h"
#include "light.h"
#include "mesh.h"

struct Scene {
  std::vector<Mesh> meshes;
  Camera camera;
  Light light;
};