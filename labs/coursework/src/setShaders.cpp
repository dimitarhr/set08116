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
	vector<string> frag_shaders{ "shaders/shader.frag", "shaders/part_direction.frag", "shaders/part_spot.frag",
		"shaders/part_point.frag" };
	basicEff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

	/*NORMAL MAPPING*/
	normalMappingEff.add_shader("shaders/shaderNormapMapping.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders_normals{ "shaders/shaderNormalMapping.frag", "shaders/part_direction.frag", "shaders/part_spot.frag",
		"shaders/part_point.frag", "shaders/part_normal_map.frag" };
	normalMappingEff.add_shader(frag_shaders_normals, GL_FRAGMENT_SHADER);

	/*SHADOWS*/
	shadows_eff.add_shader("shaders/shadow.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders_shadows{ "shaders/shadow.frag", "shaders/part_spot.frag", "shaders/part_shadow.frag" };
	shadows_eff.add_shader(frag_shaders_shadows, GL_FRAGMENT_SHADER);

	/*SKYBOX*/
	sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);

	// Build effect
	basicEff.build();
	normalMappingEff.build();
	shadows_eff.build();
	sky_eff.build();
}