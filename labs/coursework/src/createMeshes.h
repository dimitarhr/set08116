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
void RandomEggsPostions();
void createWater(int waterLevel);
void createTerrain();
void setFrameBuffers();