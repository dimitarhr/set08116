/* texturesAndNormalMaps.cpp
*  Consists of the function that loads the textures and the normal maps.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "texturesAndNormalMaps.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void loadTexturesAndNormalMaps()
{
	textures["floorPlane"] = texture("textures/stone.jpg", true, true);
	textures["earth"] = texture("textures/earth.jpg", true, true);
	textures["ring"] = texture("textures/lavatile.jpg", true, true);
	textures["ringBase"] = texture("textures/disturb.jpg", true, true);
	textures["moon"] = texture("textures/moon_sphere.jpg", true, true);
	textures["surroundings"] = texture("textures/brick.jpg", true, true);
	textures["shadowWall"] = texture("textures/grey_rocks.jpg", true, true);
	textures["sphereLeft"] = texture("textures/grey_Sphere.jpg", true, true);
	textures["dragon"] = texture("textures/dragon.jpg", true, true);
	textures["dragonEgg"] = texture("textures/dragonEgg.jpg", true, true);
	textures["stickBoxSupport"] = texture("textures/mossy_rock.jpg", true, true);
	textures["water"] = texture("textures/water.jpg", true, true);
	normal_maps["earth"] = texture("textures/earth_normalmap.jpg", true, true);
	normal_maps["floorPlane"] = texture("textures/sand_normal-map.jpg", true, true);
	normal_maps["surroundings"] = texture("textures/brick_normalmap.jpg", true, true);
	normal_maps["sphereLeft"] = texture("textures/buble_normal_map.jpg", true, true);
	normal_maps["dragonEgg"] = texture("textures/buble_normal_map.jpg", true, true);
	alpha_map = texture("textures/happymask.png");
	originalMap = texture("textures/whitescreen.jpg");
	normal_maps["water"] = texture("textures/water_normal.png", true, true);
	normal_maps["waterTwo"] = texture("textures/water_normal_Two.jpg", true, true);
	terrainTex[0] = texture("textures/sand.jpg");
	terrainTex[1] = texture("textures/grass.jpg");
	terrainTex[2] = texture("textures/stone.jpg");
	terrainTex[3] = texture("textures/snow.jpg");
}