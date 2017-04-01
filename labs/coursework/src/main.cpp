/* main.cpp
*  Consists of the main functions
*		- load_content()
*		- update()
*		- render()
*		- main()
*  that are required to render the scene.
*  There are also glabal variables definitions.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "createMeshes.h"
#include "renderMeshes.h"
#include "setCamera.h"
#include "updateFunctions.h"
#include "setLightAndMaterial.h"
#include "setShaders.h"
#include "texturesAndNormalMaps.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

/*GLOBAL VARIABLES*/
effect basicEff, normalMappingEff, shadows_eff, mask_eff, edge_eff, sepia_eff, motion_blur_eff, terrain_eff, grass_eff, water_eff;

std::array<camera*, 2> cams;
int cameraIndex = 1;
int targetCamera = 1;

shadow_map shadowMap;

map<string, mesh> normalMapMeshes, basicMeshes, shadow_geom;
std::array<mesh, 5> hierarchicalMesh;

map<string, texture> textures, normal_maps;

texture alpha_map;
texture originalMap;

directional_light dirLight;
vector<spot_light> spots(5); 
point_light pointLight;

mesh skybox;
effect sky_eff;
cubemap cube_map;

double cursor_x = 0.0;
double cursor_y = 0.0;
double velocity = 0;

frame_buffer frame;
frame_buffer temp_frame;
frame_buffer frames[2];
unsigned int current_frame = 0;
geometry screen_quad; 
geometry screen_quad_edge;
int screenMode = 0;
int edgeDetection = 0; 
int sepia = 0;
int motionBlur = 0;
int wireFrame = 0;
texture terrainTex[4];

mesh terrainMesh;
mesh grassMesh;
mesh waterMesh;

vec2 uv_scroll;
vec2 uv_scroll_Two;

const int maxGrass = 800;  

// Vector we will use to store randomly generated points
std::array<vec3, 800>offsetArray;

// Create camera objects and sets the cursor settings
bool initialise() {
	cams[0] = new target_camera();
	cams[1] = new free_camera();
	// Create shadow map
	shadowMap = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(renderer::get_window(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

// Load content 
bool load_content() {  
	     
	//////////////////////////////////////////////////////
	// Geometry to load into
	geometry geom;
	 
	// Load height map 
	texture height_map("textures/mountain_map.png");

	// Generate terrain
	generate_terrain(geom, height_map, 800, 800, 100.0f);

	// Use geometry to create terrain mesh
	terrainMesh = mesh(geom);

	terrainMesh.get_material().set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	terrainMesh.get_material().set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	terrainMesh.get_material().set_shininess(20.0f);
	terrainMesh.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	terrainTex[0] = texture("textures/sand.jpg");
	terrainTex[1] = texture("textures/grass.jpg");
	terrainTex[2] = texture("textures/stone.jpg");
	terrainTex[3] = texture("textures/snow.jpg");
	//////////////////////////////////////////////////////

	/*GRASS*/
	createGrass(height_map, 100);

	// Create 2 frame buffers - use screen width and height
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create frame buffer - use screen width and height
	frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	// *********************************
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);

	screen_quad_edge.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad_edge.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad_edge.set_type(GL_TRIANGLE_STRIP);

	/*CREATE MESHES*/
	// Meshes with normal maps - Defined in 'createMeshes.cpp'
	createNormalMapMeshes();

	// Basic meshes - Defined in 'createMeshes.cpp'
	createBasicMeshes();
	
	// Meshes with shadows - Defined in 'createMeshes.cpp'
	createShadowMeshes();
	
	// Transform hierarchy meshes - Defined in 'createMeshes.cpp'
	createHierarchicalMeshes();

	//Water
	createWater();

	// Skybox  
	skybox = mesh(geometry_builder::create_box());
	skybox.get_transform().scale = vec3(100);
	// All sides of the skybox
	array<string, 6> filenames = { "textures/sky_right.png", "textures/sky_left.png", "textures/sky_top.png", 
								   "textures/sky_botton.png", "textures/sky_front.png" ,  "textures/sky_back.png" };
	cube_map = cubemap(filenames);	
	
	/*SET MATERIAL - Defined in 'setLightAndMaterial.cpp'*/ 
	setMeshesMaterial();

	/*LOAD TEXTURES and NORMAL MAPS - Define in 'texturesAndNormalMaps.cpp'*/
	loadTexturesAndNormalMaps();

	/*DIRECTIONAL LIGHT - Defined in 'setLightAndMaterial.cpp'*/
	setDirectLight();

	/*POINT LIGHT - Defined in 'setLightAndMaterial.cpp'*/
	setPointLight();
	  
	/*SPOT LIGHT - Defined in 'setLightAndMaterial.cpp'*/ 
	setSpotLights();

	/*SHADERS - Defined in 'setShaders.cpp'*/
	setShaders();

	// Set default camera properties 
	/*FREE CAMERA*/
	cams[1]->set_position(vec3(60.0f, 45.0f, 50.0f));
	cams[1]->set_target(vec3(40.0f, 40.0f, -100.0f));
	cams[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	
	return true;
}

// Update every frame
bool update(float delta_time) {
	
	uv_scroll += vec2(0, delta_time);
	uv_scroll_Two += vec2(delta_time*2.0, delta_time); 

	// Flip frame 
	current_frame = (current_frame + 1) % 2;

	// Set the correct camera index depending on the pressed button
	// Checks if the direct light is ON or OFF
	// Defined in 'updateFunctions.cpp' 
	checkPressedButton();

	/*TARGET CAMERAS*/ 
	if (cameraIndex == 0)
	{
		// Defined in 'setCamera.cpp' 
		setTargetCamera(delta_time);
	}
	/*FREE CAMERA*/ 
	else if (cameraIndex == 1)
	{
		// Defined in 'setCamera.cpp' 
		setFreeCamera(delta_time);
	}

	// Update the shadow map properties from the spot light
	shadowMap.light_position = spots[1].get_position();
	shadowMap.light_dir = spots[1].get_direction();

	// Set skybox position to camera position (camera in centre of skybox)
	skybox.get_transform().position = cams[cameraIndex]->get_position();

	// Transform hierarchy meshes
	// Defined in 'updateFunctions.cpp' 
	updateMeshesTransformations(delta_time);

	// Accumulating velocity used for the movements
	velocity -= delta_time;

	// Update camera
	cams[cameraIndex]->update(delta_time);

	return true;
}

bool render() {
	
	// Set render target to frame buffer
	renderer::set_render_target(frame);
	// Clear frame
	renderer::clear();

	if (wireFrame == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	// Render skybox
	// Defined in 'renderMeshes.cpp' 
	renderSkyBox();
	// Render meshes with normal maps
	// Defined in 'renderMeshes.cpp' 
	renderNormalMapMesh();
	// Render meshes without normal maps
	// Defined in 'renderMeshes.cpp' 
	renderBasicMesh();

	// Render meshes with shadows 
	// Defined in 'renderMeshes.cpp' 
	renderShadowMesh();

	// Render hierarchical meshes
	// Defined in 'renderMeshes.cpp' 
	renderHierarchicalMeshes();
  
	renderWater(uv_scroll, uv_scroll_Two);
	//renderGrass(); 
	 
	renderTerrain();

	if (wireFrame == 1) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (edgeDetection == 1)
	{
		//sepia = 0;
		//motionBlur = 0;
		renderEdges();  
	}
	if (sepia == 1)
	{
		//edgeDetection = 0;
		renderSepia();
	} 

	if (motionBlur == 1) 
	{
		renderMotionBlur();
	}
	 	
	// Render the post-processing effect - MASK
	// Defined in 'renderMeshes.cpp' 
	renderMask();

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

	// Delete the object at the end
	delete cams[0];
	delete cams[1];
}