/* setLightAndMaterial.cpp
*  Consists of the functions that are required to set the lights and the materials for the different meshes.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "setLightAndMaterial.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void setSpotLights()
{
	// Spot light in the torrus, below the earth
	spots[0].set_position(vec3(25.0f, 36.0f, 10.0f));
	spots[0].set_light_colour(vec4(1.0f, 0.3f, 0.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(0, 1, -1)));
	spots[0].set_range(80);
	spots[0].set_power(0.5f);

	// The green spot light, behind the earth
	spots[1].set_position(vec3(10, 36, -8));
	spots[1].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[1].set_direction(normalize(vec3(1, 1, -1)));
	spots[1].set_range(50);
	spots[1].set_power(0.1f);

	// In front of the earth - left
	spots[2].set_position(vec3(0, 36, 40));
	spots[2].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[2].set_direction(normalize(vec3(1, 0, -1)));
	spots[2].set_range(200.0f);
	spots[2].set_power(10.0f);

	// In front of the earth - right
	spots[3].set_position(vec3(40, 36, 40));
	spots[3].set_light_colour(vec4(0.53f, 0.45f, 0.37f, 1.0f));
	spots[3].set_direction(normalize(vec3(-1, 0, -1)));
	spots[3].set_range(500.0f);
	spots[3].set_power(10.0f);

	// Spot light in front of the wall
	shadowLight.set_position(vec3(-15.5f, 10.5f, 15));
	shadowLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	shadowLight.set_direction(normalize(vec3(-1, -1, 0)));
	shadowLight.set_range(2000);
	shadowLight.set_power(0.1f);
}

void setDirectLight()
{
	dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	dirLight.set_light_colour(vec4(0.99f, 0.37f, 0.33f, 1.0f));
	dirLight.set_direction(vec3(1.0f, 1.0f, 1.0f));
}

void setPointLight() 
{
	pointLight.set_position(vec3(30, 46, -5));
	pointLight.set_light_colour(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	pointLight.set_range(20.0f);
}

void setMeshesMaterial()
{
	material objectMaterial;

	// Torch
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	objectMaterial.set_emissive(vec4(0.5f, 0.5f, 0.0f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.0f, 1.0f));
	objectMaterial.set_shininess(35);
	basicMeshes["torch"].set_material(objectMaterial);

	// Egg protectors and Dragon
	objectMaterial.set_emissive(vec4(0.1f, 0.1f, 0.0f, 1.0f));
	hierarchicalMesh[0].set_material(objectMaterial);
	hierarchicalMesh[1].set_material(objectMaterial);
	hierarchicalMesh[2].set_material(objectMaterial);
	hierarchicalMesh[3].set_material(objectMaterial);
	hierarchicalMesh[4].set_material(objectMaterial);

	// Floor
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	shadow_geom["floorPlane"].set_material(objectMaterial);

	// Earth and Moon
	normalMapMeshes["earth"].set_material(objectMaterial);
	basicMeshes["moon"].set_material(objectMaterial);

	// Dragon egg
	normalMapMeshes["dragonEgg"].set_material(objectMaterial);

	// Ring
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.0f, 1.0f));
	basicMeshes["ring"].set_material(objectMaterial);

	// Ring base
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));
	basicMeshes["ringBase"].set_material(objectMaterial);

	// Dragon
	objectMaterial.set_specular(vec4(0.4f, 0.4f, 0.4f, 1.0f));
	basicMeshes["dragon"].set_material(objectMaterial);

	// Shadow geometry
	shadow_geom["miniWall"].set_material(objectMaterial);
	shadow_geom["shadowWall"].set_material(objectMaterial);
	shadow_geom["stickBoxFront"].set_material(objectMaterial);
	shadow_geom["bigEgg"].set_material(objectMaterial);
	shadow_geom["smallEgg"].set_material(objectMaterial);

	// The rest of the surroundings
	basicMeshes["stickBoxLeft"].set_material(objectMaterial);
	basicMeshes["stickBoxRight"].set_material(objectMaterial);
	basicMeshes["stickBoxBack"].set_material(objectMaterial);
	basicMeshes["stickBoxFront"].set_material(objectMaterial);
	basicMeshes["stickBoxSupport"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxLeft"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxRight"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxBack"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxFront"].set_material(objectMaterial);
	normalMapMeshes["sphereLeft"].set_material(objectMaterial);
	waterMesh.set_material(objectMaterial);
	 
	// Terrain
	terrainMesh.get_material().set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	terrainMesh.get_material().set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	terrainMesh.get_material().set_shininess(20.0f);
	terrainMesh.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
}