#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "createMeshes.h"
#include "renderMeshes.h"
#include "setCamera.h"
#include "updateFunctions.h"
#include "setLightAndMaterial.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

/*GLOBAL VARIABLES*/
effect basicEff, normalMappingEff, shadows_eff;

std::array<camera*, 2> cams;
int cameraIndex = 1;
int targetCamera = 1;

shadow_map shadowMap;

map<string, mesh> normalMapMeshes, basicMeshes, shadow_geom;
std::array<mesh, 5> hierarchicalMesh;

map<string, texture> textures, normal_maps;

directional_light dirLight;
vector<spot_light> spots(5);
point_light pointLight;

mesh skybox;
effect sky_eff;
cubemap cube_map;

double cursor_x = 0.0;
double cursor_y = 0.0;
double velocity = 0;
 
// Create camera objects and sets the cursor settings
bool initialise() {
	cams[0] = new target_camera();
	cams[1] = new free_camera();
	// Create shadow map
	shadowMap = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

/*LOAD CONTENT FUNCTIONS*/
//void createNormalMapMeshes()
//{
//	// Create meshes
//	normalMapMeshes["floorPlane"] = mesh(geometry_builder::create_plane());
//	normalMapMeshes["earth"] = mesh(geometry_builder::create_sphere(60, 60));
//	normalMapMeshes["smallStickBoxLeft"] = mesh(geometry_builder::create_box(vec3(5.0f, 5.0f, 5.0f)));
//	normalMapMeshes["smallStickBoxRight"] = mesh(geometry_builder::create_box(vec3(4.0f, 4.0f, 4.0f)));
//	normalMapMeshes["smallStickBoxBack"] = mesh(geometry_builder::create_box(vec3(4.5f, 4.5f, 4.5f)));
//	normalMapMeshes["smallStickBoxFront"] = mesh(geometry_builder::create_box(vec3(3.0f, 3.0f, 3.0f)));
//	normalMapMeshes["sphereLeft"] = mesh(geometry_builder::create_sphere(30, 30, vec3(2.5f, 2.5f, 3.0f)));
//	normalMapMeshes["dragonEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));
//
//	// Transform meshes
//	normalMapMeshes["earth"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
//	normalMapMeshes["earth"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
//	normalMapMeshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
//	normalMapMeshes["smallStickBoxLeft"].get_transform().translate(vec3(13.0f, 2.5f, 5.0f));
//	normalMapMeshes["smallStickBoxRight"].get_transform().translate(vec3(40.0f, 2.0f, 5.0f));
//	normalMapMeshes["smallStickBoxBack"].get_transform().translate(vec3(21.0f, 2.0f, 0.0f));
//	normalMapMeshes["smallStickBoxFront"].get_transform().translate(vec3(17.0f, 1.5f, 25.0f));
//	normalMapMeshes["sphereLeft"].get_transform().translate(vec3(15.0f, 2.0f, 17.0f));
//	normalMapMeshes["dragonEgg"].get_transform().translate(vec3(25.0f, 12.0f, -30.0f));
//}

//void createBasicMeshes()
//{
//	// Create meshes
//	basicMeshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 6.5f));
//	basicMeshes["moon"] = mesh(geometry_builder::create_sphere(30, 30));
//	basicMeshes["ringBase"] = mesh(geometry_builder::create_pyramid());
//	basicMeshes["stickBoxLeft"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
//	basicMeshes["stickBoxRight"] = mesh(geometry_builder::create_box(vec3(2.5f, 10.0f, 1.0f)));
//	basicMeshes["stickBoxBack"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 1.0f)));
//	basicMeshes["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(2.5f, 5.0f, 2.5f)));
//	basicMeshes["torch"] = mesh(geometry_builder::create_cylinder());
//	basicMeshes["dragon"] = mesh((geometry("textures/dragon.obj")));
//
//	// Transform meshes
//	basicMeshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
//	basicMeshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
//	basicMeshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//	basicMeshes["ring"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
//	basicMeshes["ringBase"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
//	basicMeshes["ringBase"].get_transform().translate(vec3(25.0f, 2.5f, 10.0f));
//	basicMeshes["stickBoxLeft"].get_transform().translate(vec3(13.0f, 5.0f, 10.0f));
//	basicMeshes["stickBoxRight"].get_transform().translate(vec3(40.0f, 5.0f, 10.0f));
//	basicMeshes["stickBoxBack"].get_transform().translate(vec3(25.0f, 10.0f, 0.0f));
//	basicMeshes["stickBoxFront"].get_transform().translate(vec3(22.0f, 2.5f, 25.0f));
//	basicMeshes["torch"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
//	basicMeshes["dragon"].get_transform().scale = vec3(10);
//	basicMeshes["dragon"].get_transform().translate(vec3(18.0f, 8.0f, 15.0f));
//	basicMeshes["dragon"].get_transform().rotate(vec3(0.0f, -quarter_pi<float>(), 0.0f));
//}

//void createShadowMeshes()
//{	
//	// Create meshes
//	shadow_geom["shadowWall"] = mesh(geometry_builder::create_box(vec3(3, 15, 96)));
//	shadow_geom["miniWall"] = mesh(geometry_builder::create_box(vec3(1, 7, 10)));
//	shadow_geom["floorPlane"] = mesh(geometry_builder::create_plane(8, 100, true));
//	shadow_geom["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
//	shadow_geom["bigEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));
//	shadow_geom["smallEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(2, 1, 1)));
//
//	// Transform meshes
//	shadow_geom["shadowWall"].get_transform().translate(vec3(-56.5f, 7.5f, 0.0f));
//	shadow_geom["miniWall"].get_transform().translate(vec3(-50.5f, 3.5f, 0.0f));
//	shadow_geom["stickBoxFront"].get_transform().translate(vec3(-50.5f, 5.0f, 20.0f));
//	shadow_geom["floorPlane"].get_transform().translate(vec3(-54.0f, 0.0f, 0.0f));
//	shadow_geom["bigEgg"].get_transform().translate(vec3(-50.5f, 1.5f, -15.0f));
//	shadow_geom["bigEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f));
//	shadow_geom["smallEgg"].get_transform().translate(vec3(-48.5f, 3.5f, 0.0f));
//	shadow_geom["smallEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f));
//}

//void createHierarchicalMeshes()
//{
//	// Create meshes (From outter most to center)
//	hierarchicalMesh[0] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 10.0f));
//	hierarchicalMesh[1] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 9.0f));
//	hierarchicalMesh[2] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 8.0f));
//	hierarchicalMesh[3] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 7.0f));
//	hierarchicalMesh[4] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 6.0f));
//
//	// Transform meshes
//	hierarchicalMesh[0].get_transform().translate(vec3(25.0f, 12.0f, -30.0f));
//	hierarchicalMesh[0].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//	hierarchicalMesh[1].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//	hierarchicalMesh[2].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//	hierarchicalMesh[3].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//	hierarchicalMesh[4].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
//}

void loadTexturesAndNormalMaps()
{
	textures["floorPlane"] = texture("textures/sand.jpg", true, true);
	textures["earth"] = texture("textures/earth.jpg", true, true);
	textures["ring"] = texture("textures/lavatile.jpg", true, true);
	textures["ringBase"] = texture("textures/disturb.jpg", true, true);
	textures["moon"] = texture("textures/moon_sphere.jpg", true, true);
	textures["surroundings"] = texture("textures/brick.jpg", true, true);
	textures["shadowWall"] = texture("textures/grey_rocks.jpg", true, true);
	textures["sphereLeft"] = texture("textures/grey_Sphere.jpg", true, true);
	textures["dragon"] = texture("textures/dragon.jpg", true, true);
	textures["dragonEgg"] = texture("textures/dragonEgg.jpg", true, true);
	normal_maps["earth"] = texture("textures/earth_normalmap.jpg", true, true);
	normal_maps["floorPlane"] = texture("textures/sand_normal-map.jpg", true, true);
	normal_maps["surroundings"] = texture("textures/brick_normalmap.jpg", true, true);
	normal_maps["sphereLeft"] = texture("textures/brick_normal_map.jpg", true, true);
	normal_maps["dragonEgg"] = texture("textures/brick_normal_map.jpg", true, true);
}

//void setSpotLights()
//{
//	// Spot light in the torrus, below the earth
//	spots[0].set_position(vec3(25.0f, 0.0f, 10.0f));
//	spots[0].set_light_colour(vec4(1.0f, 0.3f, 0.0f, 1.0f));
//	spots[0].set_direction(normalize(vec3(0, 1, -1)));
//	spots[0].set_range(80);
//	spots[0].set_power(0.5f);
//
//	// Spot light in front of the wall
//	spots[1].set_position(vec3(-15.5f, 10.5f, 0));
//	spots[1].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	spots[1].set_direction(normalize(vec3(-1, -1, 0)));
//	spots[1].set_range(2000);
//	spots[1].set_power(0.1f);
//
//	// The green spot light, behind the earth
//	spots[2].set_position(vec3(10, 0, -8));
//	spots[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
//	spots[2].set_direction(normalize(vec3(1, 1, -1)));
//	spots[2].set_range(50);
//	spots[2].set_power(0.1f);
//
//	// In front of the earth - left
//	spots[3].set_position(vec3(0, 0, 40));
//	spots[3].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	spots[3].set_direction(normalize(vec3(1, 0, -1)));
//	spots[3].set_range(200.0f);
//	spots[3].set_power(10.0f);
//
//	// In front of the earth - right
//	spots[4].set_position(vec3(40, 0, 40));
//	spots[4].set_light_colour(vec4(0.53f, 0.45f, 0.37f, 1.0f));
//	spots[4].set_direction(normalize(vec3(-1, 0, -1)));
//	spots[4].set_range(500.0f);
//	spots[4].set_power(10.0f);
//}

//void setDirectLight()
//{
//	dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
//	dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	dirLight.set_direction(vec3(1.0f, 1.0f, 1.0f));
//}

//void setPointLight()
//{
//	pointLight.set_position(vec3(30, 10, -5));
//	pointLight.set_light_colour(vec4(0.0f, 1.0f, 1.0f, 1.0f));
//	pointLight.set_range(20.0f);
//}

//void setMeshesMaterial()
//{
//	material objectMaterial;
//
//	// Torch
//	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
//	objectMaterial.set_emissive(vec4(0.5f, 0.5f, 0.0f, 1.0f));
//	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.0f, 1.0f));
//	objectMaterial.set_shininess(35);
//	basicMeshes["torch"].set_material(objectMaterial);
//
//	// Egg protectors and Dragon
//	objectMaterial.set_emissive(vec4(0.1f, 0.1f, 0.0f, 1.0f));
//	hierarchicalMesh[0].set_material(objectMaterial);
//	hierarchicalMesh[1].set_material(objectMaterial);
//	hierarchicalMesh[2].set_material(objectMaterial);
//	hierarchicalMesh[3].set_material(objectMaterial);
//	hierarchicalMesh[4].set_material(objectMaterial);
//
//	// Floor
//	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
//	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
//	normalMapMeshes["floorPlane"].set_material(objectMaterial);
//	shadow_geom["floorPlane"].set_material(objectMaterial);
//
//	// Earth and Moon
//	normalMapMeshes["earth"].set_material(objectMaterial);
//	basicMeshes["moon"].set_material(objectMaterial);
//
//	// Dragon egg
//	normalMapMeshes["dragonEgg"].set_material(objectMaterial);
//
//	// Ring
//	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.0f, 1.0f));
//	basicMeshes["ring"].set_material(objectMaterial);
//
//	// Ring base
//	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
//	objectMaterial.set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));
//	basicMeshes["ringBase"].set_material(objectMaterial);
//
//	// Dragon
//	objectMaterial.set_specular(vec4(0.4f, 0.4f, 0.4f, 1.0f));
//	basicMeshes["dragon"].set_material(objectMaterial);
//
//	// Shadow geometry
//	shadow_geom["miniWall"].set_material(objectMaterial);
//	shadow_geom["shadowWall"].set_material(objectMaterial);
//	shadow_geom["stickBoxFront"].set_material(objectMaterial);
//	shadow_geom["bigEgg"].set_material(objectMaterial);
//	shadow_geom["smallEgg"].set_material(objectMaterial);
//
//	// The rest of the surroundings
//	basicMeshes["stickBoxLeft"].set_material(objectMaterial);
//	basicMeshes["stickBoxRight"].set_material(objectMaterial);
//	basicMeshes["stickBoxBack"].set_material(objectMaterial);
//	basicMeshes["stickBoxFront"].set_material(objectMaterial);
//	normalMapMeshes["smallStickBoxLeft"].set_material(objectMaterial);
//	normalMapMeshes["smallStickBoxRight"].set_material(objectMaterial);
//	normalMapMeshes["smallStickBoxBack"].set_material(objectMaterial);
//	normalMapMeshes["smallStickBoxFront"].set_material(objectMaterial);
//	normalMapMeshes["sphereLeft"].set_material(objectMaterial);
//}

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

// Load content
bool load_content() { 
	/*CREATE MESHES*/
	// Meshes with normal maps
	createNormalMapMeshes();

	// Basic meshes
	createBasicMeshes();
	
	// Meshes with shadows
	createShadowMeshes();
	
	// Transform hierarchy meshes 
	createHierarchicalMeshes();

	// Skybox
	skybox = mesh(geometry_builder::create_box());
	skybox.get_transform().scale = vec3(100);
	// All sides of the skybox
	array<string, 6> filenames = { "textures/sky_right.png", "textures/sky_left.png", "textures/sky_top.png", 
								   "textures/sky_botton.png", "textures/sky_front.png" ,  "textures/sky_back.png" };
	cube_map = cubemap(filenames);	
	
	/*SET MATERIAL*/
	setMeshesMaterial();

	/*LOAD TEXTURES and NORMAL MAPS*/
	loadTexturesAndNormalMaps();

	/*DIRECTIONAL LIGHT*/
	setDirectLight();

	/*POINT LIGHT*/
	setPointLight();
	  
	/*SPOT LIGHT*/ 
	setSpotLights();

	/*SHADERS*/
	setShaders();

	// Set default camera properties 
	/*FREE CAMERA*/
	cams[1]->set_position(vec3(40.0f, 10.0f, 50.0f));
	cams[1]->set_target(vec3(0.0f, 0.0f, -100.0f));
	cams[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	return true;
}

/*UPDATE FUNCTIONS*/
//void checkPressedButton()
//{
//	if (glfwGetKey(renderer::get_window(), 'F'))
//	{
//		cameraIndex = 1;
//	}
//
//	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_1))
//	{
//		cameraIndex = 0;
//		targetCamera = 1;
//	}
//	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_2))
//	{
//		cameraIndex = 0;
//		targetCamera = 2;
//	}
//	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_3))
//	{
//		cameraIndex = 0;
//		targetCamera = 3;
//	}
//	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_4))
//	{
//		cameraIndex = 0;
//		targetCamera = 4;
//	}
//
//	// Turn on/off the direct light
//	if (glfwGetKey(renderer::get_window(), 'L'))
//	{
//		// Directional light OFF
//		dirLight.set_ambient_intensity(vec4(0.0f, 0.0f, 0.0f, 1.0f));
//		dirLight.set_light_colour(vec4(0.0f, 0.0f, 0.0f, 1.0f));
//	}
//	else if (glfwGetKey(renderer::get_window(), 'M'))
//	{
//		// Directional light ON
//		dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
//		dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//}

//void setTargetCamera(float delta_time)
//{
//	if (targetCamera == 1) {
//		cams[0]->set_position(vec3(50.0f, 50.0f, 50.0f));
//		cams[0]->set_target(vec3(10.0f, 0.0f, 10.0f));
//	}
//	else if (targetCamera == 2) {
//		cams[0]->set_position(vec3(-50.0f, 50.0f, 50.0f));
//		cams[0]->set_target(vec3(10.0f, 0.0f, 10.0f));
//	}
//	else if (targetCamera == 3) {
//		cams[0]->set_position(vec3(-10.0f, 100.0f, 0.0f));
//		cams[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
//	}
//	else if (targetCamera == 4) {
//		cams[0]->set_position(vec3(10.0f, 10.0f, 0.0f));
//		cams[0]->set_target(vec3(-100.0f, 0.0f, 0.0f));
//	}
//	cams[0]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);
//	cams[0]->update(delta_time);
//}

//void setFreeCamera(float delta_time)
//{
//	double current_x = 0;
//	double current_y = 0;
//	double delta_x;
//	double delta_y;
//	vec3 pos = vec3(0);
//
//	// The ratio of pixels to rotation
//	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
//	static double ratio_height = (quarter_pi<float>() *
//		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
//		static_cast<float>(renderer::get_screen_height());
//
//	// Get the current cursor position
//	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
//
//	// Calculate delta of cursor positions from last frame
//	delta_x = current_x - cursor_x;
//	delta_y = current_y - cursor_y;
//	// Multiply deltas by ratios - gets actual change in orientation
//	delta_x = delta_x * ratio_width;
//	delta_y = delta_y * ratio_height;
//	// Rotate camera by delta
//	static_cast<free_camera*>(cams[1])->rotate(delta_x, -delta_y);
//
//	// Use keyboard to move the camera
//	if (glfwGetKey(renderer::get_window(), 'W')) {
//		pos += vec3(0.0f, 0.0f, 20.0f) * delta_time;
//	}
//	if (glfwGetKey(renderer::get_window(), 'S')) {
//		pos -= vec3(0.0f, 0.0f, 20.0f) * delta_time;
//	}
//	if (glfwGetKey(renderer::get_window(), 'A')) {
//		pos -= vec3(20.0f, 0.0f, 0.0f) * delta_time;
//	}
//	if (glfwGetKey(renderer::get_window(), 'D')) {
//		pos += vec3(20.0f, 0.0f, 0.0f) * delta_time;
//	}
//
//	// Move camera
//	static_cast<free_camera*>(cams[1])->move(pos);
//	// Update the camera
//	static_cast<free_camera*>(cams[1])->update(delta_time);
//
//	// Update cursor pos
//	cursor_x = current_x;
//	cursor_y = current_y;
//}

//void updateMeshesTransformations(float delta_time)
//{
//	// Using the cos and sin functions to calculate the range of movement
//	vec3 moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
//	vec3 levitatingRange = vec3(0.0f, cos(velocity)*1.5f, 0.0f);
//
//	hierarchicalMesh[0].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
//	hierarchicalMesh[1].get_transform().rotate(vec3(quarter_pi<float>(), 0.0f, 0.0f) * delta_time);
//	hierarchicalMesh[2].get_transform().rotate(vec3(0.0f, quarter_pi<float>(), 0.0f) * delta_time);
//	hierarchicalMesh[3].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
//	hierarchicalMesh[4].get_transform().rotate(vec3(quarter_pi<float>(), 0.0f, 0.0f) * delta_time);
//
//	// Rotate the Earth and the Moon around their Z axis
//	normalMapMeshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
//	basicMeshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
//
//	// Rotating moon around the Earth
//	basicMeshes["moon"].get_transform().position = moonPos + normalMapMeshes["earth"].get_transform().position;
//
//	// Move the spot light in front of the wall
//	spots[1].set_position(vec3(-42.9f, 5.5f, sin(velocity) * -40));
//	basicMeshes["torch"].get_transform().position = vec3(-38.0f, 5.5f, sin(velocity) * -40);
//
//	// Levitating egg and protectors
//	normalMapMeshes["dragonEgg"].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;
//	hierarchicalMesh[0].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;
//}

// Update every frame
bool update(float delta_time) {
	//vec3 moonPos = vec3(0);
	//vec3 levitatingRange = vec3(0);

	// Set the correct camera index depending on the pressed button
	// Checks if the direct light is ON or OFF
	checkPressedButton();

	/*TARGET CAMERAS*/
	if (cameraIndex == 0)
	{
		setTargetCamera(delta_time);
	}
	/*FREE CAMERA*/
	else if (cameraIndex == 1)
	{
		setFreeCamera(delta_time);
	}

	// Rotate the Earth and the Moon around their Z axis
	/*normalMapMeshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	basicMeshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);*/

	//// Using the cos and sin functions to calculate the range of movement
	//moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
	//levitatingRange = vec3(0.0f, cos(velocity)*1.5f, 0.0f);

	//// Rotating moon around the Earth
	//basicMeshes["moon"].get_transform().position = moonPos + normalMapMeshes["earth"].get_transform().position;
	//
	//// Move the spot light in front of the wall
	//spots[1].set_position(vec3(-42.9f, 5.5f, sin(velocity) * -40));
	//basicMeshes["torch"].get_transform().position = vec3(-38.0f, 5.5f, sin(velocity) * -40);

	// Update the shadow map properties from the spot light
	shadowMap.light_position = spots[1].get_position();
	shadowMap.light_dir = spots[1].get_direction();

	// Set skybox position to camera position (camera in centre of skybox)
	skybox.get_transform().position = cams[cameraIndex]->get_position();

	// Transform hierarchy meshes
	updateMeshesTransformations(delta_time);

	// Accumulating velocity used for the movements
	velocity -= delta_time;

	//// Levitating egg and protectors
	//normalMapMeshes["dragonEgg"].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;
	//hierarchicalMesh[0].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;

	return true;
}

/*RENDER FUNCTIONS*/
// Render meshes with normal maps
//void renderNormalMapMesh()
//{
//	// Bing effect
//	renderer::bind(normalMappingEff);
//
//	// Bind lights
//	renderer::bind(dirLight, "light");
//	renderer::bind(pointLight, "pointLight");
//	renderer::bind(spots, "spots");
//
//	for (auto &item : normalMapMeshes) {
//		// Gets the mesh
//		auto geometryItem = item.second;
//		// Gets the name of the mesh
//		string geometryName = item.first;
//		// Normal matrix
//		auto N = geometryItem.get_transform().get_normal_matrix();
//		// Create MVP matrix
//		auto M = geometryItem.get_transform().get_transform_matrix();
//		auto V = cams[cameraIndex]->get_view();
//		auto P = cams[cameraIndex]->get_projection();
//		auto MVP = P * V * M;
//		 
//		// Set MVP matrix uniform
//		glUniformMatrix4fv(normalMappingEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
//
//		// Set M matrix uniform
//		glUniformMatrix4fv(normalMappingEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
//
//		// Set N matrix uniform
//		glUniformMatrix3fv(normalMappingEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
//
//		// Bind material
//		renderer::bind(geometryItem.get_material(), "mat");
//
//		// Bind texture
//		if (textures.find(geometryName) != textures.end())
//		{
//			renderer::bind(textures[geometryName], 0);
//		}
//		else
//		{
//			renderer::bind(textures["surroundings"], 0);
//		}
//
//		// Bind normal map
//		if (normal_maps.find(geometryName) != normal_maps.end())
//		{
//			renderer::bind(normal_maps[geometryName], 1);
//		}
//		else
//		{
//			renderer::bind(normal_maps["surroundings"], 1);
//		}
//
//		// Set the texture uniform value
//		glUniform1i(normalMappingEff.get_uniform_location("tex"), 0);
//
//		// Set the normal_map uniform value
//		glUniform1i(normalMappingEff.get_uniform_location("normal_map"), 1);
//
//		// Set the viewer position uniform value
//		glUniform3fv(normalMappingEff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
//
//		// Render geometry
//		renderer::render(geometryItem);
//	}
//}

// Render basic meshes
//void renderBasicMesh()
//{
//	// Bind effect
//	renderer::bind(basicEff);
//
//	// Bind lights
//	renderer::bind(dirLight, "light");
//	renderer::bind(pointLight, "pointLight");
//	renderer::bind(spots, "spots");
//
//	for (auto &item : basicMeshes) {
//		// Gets the mesh
//		auto geometryItem = item.second;
//		// Gets the name of the mesh
//		string geometryName = item.first;
//		// Normal matrix
//		auto N = geometryItem.get_transform().get_normal_matrix();
//		// Create MVP matrix
//		auto M = geometryItem.get_transform().get_transform_matrix();
//		auto V = cams[cameraIndex]->get_view();
//		auto P = cams[cameraIndex]->get_projection();
//		auto MVP = P * V * M;
//
//		// Set MVP matrix uniform
//		glUniformMatrix4fv(basicEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
//
//		// Set M matrix uniform
//		glUniformMatrix4fv(basicEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
//
//		// Set N matrix uniform
//		glUniformMatrix3fv(basicEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
//
//		// Bind material
//		renderer::bind(geometryItem.get_material(), "mat");
//
//		// Bind texture
//		if (textures.find(geometryName) != textures.end())
//		{
//			renderer::bind(textures[geometryName], 0);
//		}
//		else
//		{
//			renderer::bind(textures["surroundings"], 0);
//		}
//
//		// Set the texture uniform value
//		glUniform1i(basicEff.get_uniform_location("tex"), 0);
//		
//		// Set the viewer position uniform value
//		glUniform3fv(basicEff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
//
//		// Render geometry
//		renderer::render(geometryItem);
//	}
//}

// Render meshes with shadows
//void renderShadowMesh() 
//{
//	// Set render target to shadow map
//	renderer::set_render_target(shadowMap);
//	// Clear depth buffer bit
//	glClear(GL_DEPTH_BUFFER_BIT);
//	// Set render mode to cull face
//	glCullFace(GL_FRONT);
//	// Create a new projection matrix for the light
//	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);
//
//	// Bind shader
//	renderer::bind(basicEff);
//
//	// Render meshes
//	for (auto &e : shadow_geom) {
//		// Gets every mesh
//		auto m = e.second;
//		// Create MVP matrix
//		auto M = m.get_transform().get_transform_matrix();
//		// View matrix taken from shadow map
//		auto V = shadowMap.get_view();
//		// Calculate the MVP matrix
//		auto MVP = LightProjectionMat * V * M;
//		// Set MVP matrix uniform
//		glUniformMatrix4fv(basicEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));		
//		// Render mesh
//		renderer::render(m);
//	}
//	// Set render target back to the screen
//	renderer::set_render_target();
//	// Set cull face to back
//	glCullFace(GL_BACK);
//
//	// Bind shader
//	renderer::bind(shadows_eff);
//
//	// Render meshes
//	for (auto &e : shadow_geom) {
//		// Gets every mesh
//		auto m = e.second;
//		// Gets the name of every mesh
//		string geometryName = e.first;
//		// Create MVP matrix
//		auto M = m.get_transform().get_transform_matrix();
//		auto V = cams[cameraIndex]->get_view();
//		auto P = cams[cameraIndex]->get_projection();
//		auto MVP = P * V * M;
//		// Set MVP matrix uniform
//		glUniformMatrix4fv(shadows_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
//		
//		// Set M matrix uniform
//		glUniformMatrix4fv(shadows_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
//		// Set N matrix uniform
//		glUniformMatrix3fv(shadows_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
//
//		// Set light transform
//		auto lM = m.get_transform().get_transform_matrix();
//		auto lV = shadowMap.get_view();
//		auto lightMVP = LightProjectionMat * lV * lM;
//		glUniformMatrix4fv(shadows_eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
//		// Bind material
//		renderer::bind(m.get_material(), "mat");
//		// Bind spot light
//		renderer::bind(spots[1], "spot");
//		// Bind texture
//		if (textures.find(geometryName) != textures.end())
//		{
//			renderer::bind(textures[geometryName], 0);
//		}
//		else
//		{
//			renderer::bind(textures["surroundings"], 0);
//		}
//		// Set tex uniform
//		glUniform1i(shadows_eff.get_uniform_location("tex"), 0);
//		// Set eye position
//		glUniform3fv(shadows_eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
//		// Bind shadow map texture
//		renderer::bind(shadowMap.buffer->get_depth(), 1);
//		glUniform1i(shadows_eff.get_uniform_location("shadow_map"), 1);
//		// Render mesh
//		renderer::render(m);
//	}
//}

// Render skybox
//void renderSkyBox()
//{
//	// Disable depth test,depth mask,face culling
//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);
//	glDisable(GL_CULL_FACE);
//
//	// Bind effect
//	renderer::bind(sky_eff);
//
//	// Calculate MVP for the skybox
//	auto M = skybox.get_transform().get_transform_matrix();
//	auto V = cams[cameraIndex]->get_view();
//	auto P = cams[cameraIndex]->get_projection();
//	auto MVP = P * V * M;
//	renderer::bind(cube_map, 0);
//
//	// Set MVP matrix uniform
//	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
//	
//	// Set cubemap uniform
//	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);
//
//	// Render skybox
//	renderer::render(skybox); 
//
//	// Enable depth test,depth mask,face culling
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//	glEnable(GL_CULL_FACE);
//}

// Render hierarchical meshes
//void renderHierarchicalMeshes() 
//{
//	// Bind effect
//	renderer::bind(basicEff);
//
//	// Bind lights
//	renderer::bind(dirLight, "light");
//	renderer::bind(pointLight, "pointLight");
//	renderer::bind(spots, "spots");
//
//	// Get PV
//	const auto PV = cams[cameraIndex]->get_projection() * cams[cameraIndex]->get_view();
//	// Set the texture value for the shader here
//	glUniform1i(basicEff.get_uniform_location("tex"), 0);
//	// Find the lcoation for the MVP uniform
//	const auto loc = basicEff.get_uniform_location("MVP");
//
//	// Render meshes
//	for (size_t i = 0; i < hierarchicalMesh.size(); i++) {
//		// Normal matrix
//		auto N = hierarchicalMesh[i].get_transform().get_normal_matrix();
//		// SET M to be the mesh transform matrix
//		auto M = hierarchicalMesh[i].get_transform().get_transform_matrix();
//		// Set M matrix uniform
//		glUniformMatrix4fv(basicEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
//		// Set N matrix uniform
//		glUniformMatrix3fv(basicEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
//		// Apply the heirarchy chain
//		for (size_t j = i; j > 0; j--) 
//		{
//			M = hierarchicalMesh[j - 1].get_transform().get_transform_matrix() * M;
//		}
//		// Bind material
//		renderer::bind(hierarchicalMesh[i].get_material(), "mat");
//		// Set MVP matrix uniform
//		glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(PV * M));
//		// Bind texture to renderer
//		renderer::bind(textures["ring"], 0);
//		// Render mesh
//		renderer::render(hierarchicalMesh[i]);
//	}
//}

bool render() {
	
	// Render skybox
	renderSkyBox();

	// Render meshes with normal maps
	renderNormalMapMesh();

	// Render meshes without normal maps
	renderBasicMesh();

	// Render meshes with shadows 
	renderShadowMesh();

	// Render hierarchical meshes
	renderHierarchicalMeshes();

	return true;
}

void main() {
	// Create application
	app application("Graphics Coursework");
  
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();

	delete cams[0];
	delete cams[1];
}