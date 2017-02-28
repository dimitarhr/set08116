#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "createMeshes.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void createNormalMapMeshes()
{
	// Create meshes
	normalMapMeshes["floorPlane"] = mesh(geometry_builder::create_plane());
	normalMapMeshes["earth"] = mesh(geometry_builder::create_sphere(60, 60));
	normalMapMeshes["smallStickBoxLeft"] = mesh(geometry_builder::create_box(vec3(5.0f, 5.0f, 5.0f)));
	normalMapMeshes["smallStickBoxRight"] = mesh(geometry_builder::create_box(vec3(4.0f, 4.0f, 4.0f)));
	normalMapMeshes["smallStickBoxBack"] = mesh(geometry_builder::create_box(vec3(4.5f, 4.5f, 4.5f)));
	normalMapMeshes["smallStickBoxFront"] = mesh(geometry_builder::create_box(vec3(3.0f, 3.0f, 3.0f)));
	normalMapMeshes["sphereLeft"] = mesh(geometry_builder::create_sphere(30, 30, vec3(2.5f, 2.5f, 3.0f)));
	normalMapMeshes["dragonEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));

	// Transform meshes
	normalMapMeshes["earth"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	normalMapMeshes["earth"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	normalMapMeshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	normalMapMeshes["smallStickBoxLeft"].get_transform().translate(vec3(13.0f, 2.5f, 5.0f));
	normalMapMeshes["smallStickBoxRight"].get_transform().translate(vec3(40.0f, 2.0f, 5.0f));
	normalMapMeshes["smallStickBoxBack"].get_transform().translate(vec3(21.0f, 2.0f, 0.0f));
	normalMapMeshes["smallStickBoxFront"].get_transform().translate(vec3(17.0f, 1.5f, 25.0f));
	normalMapMeshes["sphereLeft"].get_transform().translate(vec3(15.0f, 2.0f, 17.0f));
	normalMapMeshes["dragonEgg"].get_transform().translate(vec3(25.0f, 12.0f, -30.0f));
}

void createBasicMeshes()
{
	// Create meshes
	basicMeshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 6.5f));
	basicMeshes["moon"] = mesh(geometry_builder::create_sphere(30, 30));
	basicMeshes["ringBase"] = mesh(geometry_builder::create_pyramid());
	basicMeshes["stickBoxLeft"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
	basicMeshes["stickBoxRight"] = mesh(geometry_builder::create_box(vec3(2.5f, 10.0f, 1.0f)));
	basicMeshes["stickBoxBack"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 1.0f)));
	basicMeshes["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(2.5f, 5.0f, 2.5f)));
	basicMeshes["torch"] = mesh(geometry_builder::create_cylinder());
	basicMeshes["dragon"] = mesh((geometry("textures/dragon.obj")));

	// Transform meshes
	basicMeshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
	basicMeshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	basicMeshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	basicMeshes["ring"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	basicMeshes["ringBase"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	basicMeshes["ringBase"].get_transform().translate(vec3(25.0f, 2.5f, 10.0f));
	basicMeshes["stickBoxLeft"].get_transform().translate(vec3(13.0f, 5.0f, 10.0f));
	basicMeshes["stickBoxRight"].get_transform().translate(vec3(40.0f, 5.0f, 10.0f));
	basicMeshes["stickBoxBack"].get_transform().translate(vec3(25.0f, 10.0f, 0.0f));
	basicMeshes["stickBoxFront"].get_transform().translate(vec3(22.0f, 2.5f, 25.0f));
	basicMeshes["torch"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
	basicMeshes["dragon"].get_transform().scale = vec3(10);
	basicMeshes["dragon"].get_transform().translate(vec3(18.0f, 8.0f, 15.0f));
	basicMeshes["dragon"].get_transform().rotate(vec3(0.0f, -quarter_pi<float>(), 0.0f));
}

void createShadowMeshes()
{
	// Create meshes
	shadow_geom["shadowWall"] = mesh(geometry_builder::create_box(vec3(3, 15, 96)));
	shadow_geom["miniWall"] = mesh(geometry_builder::create_box(vec3(1, 7, 10)));
	shadow_geom["floorPlane"] = mesh(geometry_builder::create_plane(8, 100));
	shadow_geom["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
	shadow_geom["bigEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));
	shadow_geom["smallEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(2, 1, 1)));

	// Transform meshes
	shadow_geom["shadowWall"].get_transform().translate(vec3(-56.5f, 7.5f, 0.0f));
	shadow_geom["miniWall"].get_transform().translate(vec3(-50.5f, 3.5f, 0.0f));
	shadow_geom["stickBoxFront"].get_transform().translate(vec3(-50.5f, 5.0f, 20.0f));
	shadow_geom["floorPlane"].get_transform().translate(vec3(-54.0f, 0.0f, 0.0f));
	shadow_geom["bigEgg"].get_transform().translate(vec3(-50.5f, 1.5f, -15.0f));
	shadow_geom["bigEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f));
	shadow_geom["smallEgg"].get_transform().translate(vec3(-48.5f, 3.5f, 0.0f));
	shadow_geom["smallEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f));
}

void createHierarchicalMeshes()
{
	// Create meshes (From outter most to center)
	hierarchicalMesh[0] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 10.0f));
	hierarchicalMesh[1] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 9.0f));
	hierarchicalMesh[2] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 8.0f));
	hierarchicalMesh[3] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 7.0f));
	hierarchicalMesh[4] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 6.0f));

	// Transform meshes
	hierarchicalMesh[0].get_transform().translate(vec3(25.0f, 12.0f, -30.0f));
	hierarchicalMesh[0].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[1].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[2].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[3].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[4].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
}