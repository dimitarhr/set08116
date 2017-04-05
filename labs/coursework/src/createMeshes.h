#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

void createNormalMapMeshes();
void createBasicMeshes();
void createShadowMeshes();
void createHierarchicalMeshes();
void createGrass(const texture &height_map, float height_scale);
void createWater();
void createTerrain();
void setFrameBuffers();