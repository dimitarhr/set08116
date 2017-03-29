#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

/*GLOBAL VARIABLES*/
// Effects
extern effect basicEff, normalMappingEff, shadows_eff, sky_eff, mask_eff, edge_eff, sepia_eff, motion_blur_eff;

// Meshes
extern map<string, mesh> normalMapMeshes, basicMeshes, shadow_geom;
extern std::array<mesh, 5> hierarchicalMesh;
extern mesh skybox;

// Lights
extern directional_light dirLight;
extern vector<spot_light> spots;
extern point_light pointLight;

// Cameras
extern std::array<camera*, 2> cams;

// Camera indexes
extern int cameraIndex, targetCamera;

// Coordinates used for the Free camera
extern double cursor_x, cursor_y;

// Velocity used for the moving objects
extern double velocity;

// Textures and Normal maps
extern map<string, texture> textures, normal_maps;

extern texture alpha_map;
extern texture originalMap;

// Shadow map
extern shadow_map shadowMap;

// Cube map
extern cubemap cube_map;

extern frame_buffer frame;
extern frame_buffer temp_frame;
extern geometry screen_quad;
extern int screenMode;
extern int edgeDetection;
extern int sepia;
extern int motionBlur;

extern frame_buffer frames[2];
extern unsigned int current_frame;