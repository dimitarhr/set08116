/* setShaders.cpp
*  Consists of the function that is required to set the effects with the different shaders.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "setShaders.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void setShaders()
{
	/*BASIC EFFECTS*/
	basicEff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders{ "shaders/shader.frag", "shaders/part_direction.frag", "shaders/part_spot.frag", "shaders/part_point.frag" };
	basicEff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

	/*NORMAL MAPPING*/
	normalMappingEff.add_shader("shaders/shaderNormapMapping.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders_normals{ "shaders/shaderNormalMapping.frag", "shaders/part_direction.frag", "shaders/part_spot.frag", "shaders/part_point.frag", "shaders/part_normal_map.frag" };
	normalMappingEff.add_shader(frag_shaders_normals, GL_FRAGMENT_SHADER);

	/*SHADOWS*/
	shadows_eff.add_shader("shaders/shadow.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders_shadows{ "shaders/shadow.frag", "shaders/part_spot.frag", "shaders/part_shadow.frag" };
	shadows_eff.add_shader(frag_shaders_shadows, GL_FRAGMENT_SHADER);

	/*SKYBOX*/
	sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);

	/*MASK*/
	mask_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	mask_eff.add_shader("shaders/mask.frag", GL_FRAGMENT_SHADER);

	/*EDGE DETECTION*/ 
	edge_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	edge_eff.add_shader("shaders/edge.frag", GL_FRAGMENT_SHADER);

	/*SEPIA*/
	sepia_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	sepia_eff.add_shader("shaders/greyscale.frag", GL_FRAGMENT_SHADER);

	/*MOTION BLUR*/
	motion_blur_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	motion_blur_eff.add_shader("shaders/motion_blur.frag", GL_FRAGMENT_SHADER);
	
	/*TERRAIN*/
	terrain_eff.add_shader("shaders/terrain.vert", GL_VERTEX_SHADER);
	terrain_eff.add_shader("shaders/terrain.frag", GL_FRAGMENT_SHADER);
	terrain_eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	terrain_eff.add_shader("shaders/part_weighted_texture_4.frag", GL_FRAGMENT_SHADER);

	/*GRASS*/
	// Load in shaders
	grass_eff.add_shader("shaders/grassVertex.vert", GL_VERTEX_SHADER);
	grass_eff.add_shader("shaders/grassFrag.frag", GL_FRAGMENT_SHADER);
	grass_eff.add_shader("shaders/grassGeom.geom", GL_GEOMETRY_SHADER);

	/*WATER*/
	// Load in shaders
	water_eff.add_shader("shaders/waterVertex.vert", GL_VERTEX_SHADER);
	vector<string> frag_water{ "shaders/waterFrag.frag", "shaders/part_direction.frag", "shaders/part_spot.frag", "shaders/part_point.frag", "shaders/part_normal_map.frag" };
	water_eff.add_shader(frag_water, GL_FRAGMENT_SHADER);
	//water_eff.add_shader("shaders/grassGeom.geom", GL_GEOMETRY_SHADER);

	// Build effect
	basicEff.build();
	normalMappingEff.build();
	shadows_eff.build();
	sky_eff.build();
	mask_eff.build();
	edge_eff.build();
	sepia_eff.build();
	motion_blur_eff.build();
	terrain_eff.build();
	grass_eff.build();
	water_eff.build();
}