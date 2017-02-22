#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

effect basicEff, normalMappingEff, shadows_eff;
std::array<camera*, 2> cams;
int cameraIndex = 1;
int targetCamera = 1;
map<string, mesh> meshes;
shadow_map shadow;
map<string, mesh> normalMapMeshes;
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, mesh> shadow_geom;
directional_light dirLight;
vector<spot_light> spots(5);
vector<point_light> points(4);

double cursor_x = 0.0;
double cursor_y = 0.0;
double velocity = 0;
 
// before load_content
bool initialise() {
	cams[0] = new target_camera();
	cams[1] = new free_camera();
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

bool load_content() {
	// Create shadow map
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	// Create meshes
	normalMapMeshes["floorPlane"] = mesh(geometry_builder::create_plane());
	meshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 6.5f));
	normalMapMeshes["earth"] = mesh(geometry_builder::create_sphere(60, 60));
	meshes["moon"] = mesh(geometry_builder::create_sphere(30, 30));
	meshes["ringBase"] = mesh(geometry_builder::create_pyramid());
	meshes["stickBoxLeft"] = mesh(geometry_builder::create_box(vec3(0.2f,2.0f,0.2f)));
	normalMapMeshes["smallStickBoxLeft"] = mesh(geometry_builder::create_box(vec3(5.0f, 5.0f, 5.0f)));
	meshes["stickBoxRight"] = mesh(geometry_builder::create_box(vec3(0.5f, 2.0f, 0.2f)));
	normalMapMeshes["smallStickBoxRight"] = mesh(geometry_builder::create_box(vec3(4.0f, 4.0f, 4.0f)));
	meshes["stickBoxBack"] = mesh(geometry_builder::create_box(vec3(0.2f, 4.0f, 0.2f)));
	normalMapMeshes["smallStickBoxBack"] = mesh(geometry_builder::create_box(vec3(4.5f, 4.5f, 4.5f)));
	meshes["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(0.5f, 1.0f, 0.5f)));
	normalMapMeshes["smallStickBoxFront"] = mesh(geometry_builder::create_box(vec3(3.0f, 3.0f, 3.0f)));
	normalMapMeshes["sphereLeft"] = mesh(geometry_builder::create_sphere(30, 30));
	// Under testing
	shadow_geom["wall"] = mesh(geometry_builder::create_box(vec3(1,7,32)));
	meshes["torch"] = mesh(geometry_builder::create_cylinder());
	shadow_geom["stick"] = mesh(geometry_builder::create_box(vec3(1, 7, 10)));

	// Transform objects
	normalMapMeshes["earth"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	normalMapMeshes["earth"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	normalMapMeshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["moon"].get_transform().scale = vec3(0.9f, 0.9f, 0.9f);
	meshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
	meshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	meshes["ring"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	meshes["ringBase"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["ringBase"].get_transform().translate(vec3(25.0f, 2.5f, 10.0f));
	meshes["stickBoxLeft"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxLeft"].get_transform().translate(vec3(13.0f, 4.0f, 10.0f));
	normalMapMeshes["smallStickBoxLeft"].get_transform().translate(vec3(13.0f, 2.5f, 5.0f));
	meshes["stickBoxRight"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxRight"].get_transform().translate(vec3(40.0f, 5.0f, 10.0f));
	normalMapMeshes["smallStickBoxRight"].get_transform().translate(vec3(40.0f, 2.0f, 5.0f));
	meshes["stickBoxBack"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxBack"].get_transform().translate(vec3(25.0f, 9.5f, 0.0f));
	normalMapMeshes["smallStickBoxBack"].get_transform().translate(vec3(21.0f, 2.0f, 0.0f));
	meshes["stickBoxFront"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxFront"].get_transform().translate(vec3(22.0f, 2.5f, 25.0f));
	normalMapMeshes["smallStickBoxFront"].get_transform().translate(vec3(17.0f, 1.5f, 25.0f));
	normalMapMeshes["sphereLeft"].get_transform().translate(vec3(15.0f, 2.0f, 17.0f));
	normalMapMeshes["sphereLeft"].get_transform().scale = vec3(3.0f, 2.5f, 3.0f);
	// Under testing 
	shadow_geom["wall"].get_transform().scale = vec3(3.0f, 3.0f, 3.0f);
	shadow_geom["wall"].get_transform().translate(vec3(-48.5f, 10.5f, 0.0f));
	shadow_geom["stick"].get_transform().translate(vec3(-40.5f, 10.5f, 0.0f));
	meshes["torch"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
	meshes["torch"].get_transform().translate(vec3(-25.5f, 10.5f, -40));

	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25 
	// Earth
	material objectMaterial;  
	 
	// Torch
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	objectMaterial.set_emissive(vec4(0.5f, 0.5f, 0.0f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.0f, 1.0f));
	objectMaterial.set_shininess(35);
	meshes["torch"].set_material(objectMaterial);

	// Floor
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	normalMapMeshes["floorPlane"].set_material(objectMaterial);

	// Earth
	normalMapMeshes["earth"].set_material(objectMaterial);
	
	// Moon 
	meshes["moon"].set_material(objectMaterial);

	// Ring
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.0f, 1.0f));
	meshes["ring"].set_material(objectMaterial);

	// Wall
	objectMaterial.set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));
	shadow_geom["wall"].set_material(objectMaterial);

	// ringBase
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["ringBase"].set_material(objectMaterial);
	   
	// The rest of the surroundings
	objectMaterial.set_specular(vec4(0.4f, 0.4f, 0.4f, 1.0f));
	meshes["stickBoxLeft"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxLeft"].set_material(objectMaterial);
	meshes["stickBoxRight"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxRight"].set_material(objectMaterial);
	meshes["stickBoxBack"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxBack"].set_material(objectMaterial);
	normalMapMeshes["smallStickBoxFront"].set_material(objectMaterial);
	meshes["stickBoxFront"].set_material(objectMaterial); 
	normalMapMeshes["sphereLeft"].set_material(objectMaterial);

	// Load texture 
	textures["floorPlane"] = texture("textures/sand.jpg",true,true);
	textures["pool"] = texture("textures/sand.jpg", true, true);
	textures["earth"] = texture("textures/earth.jpg", true, true);
	textures["ring"] = texture("textures/lavatile.jpg", true, true); 
	textures["ringBase"] = texture("textures/disturb.jpg", true, true);
	textures["moon"] = texture("textures/moon_sphere.jpg", true, true);
	textures["surroundings"] = texture("textures/brick.jpg", true, true);
	textures["wall"] = texture("textures/grey_rocks.jpg", true, true);
	textures["sphereLeft"] = texture("textures/brick_diffuse.jpg", true, true);
	normal_maps["earth"] = texture("textures/earth_normalmap.jpg", true, true);
	normal_maps["rocks_normal_map"] = texture("textures/rock_norma_map.jpg", true, true);
	normal_maps["floorPlane"] = texture("textures/sand_normal-map.jpg", true, true);
	normal_maps["surroundings"] = texture("textures/brick_normalmap.jpg", true, true);
	normal_maps["sphereLeft"] = texture("textures/brick_normal_map.jpg", true, true);

	/*DIRECTIONAL LIGHT*/
	// ambient intensity 
	dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	// Light colour
	dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	// Light direction
	dirLight.set_direction(vec3(1.0f, 1.0f, 1.0f));

	/*POINT LIGHT*/
	points[0].set_position(vec3(30, 10, -5));
	points[0].set_light_colour(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	points[0].set_range(20.0f);
	  
	/*SPOT LIGHT*/ 
	// Spot light in the torrus
	spots[0].set_position(vec3(25.0f, 0.0f, 10.0f));
	spots[0].set_light_colour(vec4(1.0f, 0.3f, 0.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(0, 1, -1)));
	spots[0].set_range(80);
	spots[0].set_power(0.5f);

	// Spot light in front of the wall
	spots[1].set_position(vec3(-35.5f, 10.5f, 0));
	spots[1].set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	spots[1].set_direction(normalize(spots[1].get_position()));
	spots[1].set_range(200);
	spots[1].set_power(0.1f);
	
	// The green spot light
	spots[2].set_position(vec3(10, 0, -8));
	spots[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[2].set_direction(normalize(vec3(1, 1, -1)));
	spots[2].set_range(50);
	spots[2].set_power(0.1f);

	// Front left
	spots[3].set_position(vec3(0, 0, 40)); 
	spots[3].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[3].set_direction(normalize(vec3(1, 0, -1)));
	spots[3].set_range(200.0f);
	spots[2].set_power(0.1f);

	// Front right
	spots[4].set_position(vec3(40, 0, 40));
	spots[4].set_light_colour(vec4(0.53f, 0.45f, 0.37f, 1.0f));
	spots[4].set_direction(normalize(vec3(-1, 0, -1)));
	spots[4].set_range(500.0f);
	spots[4].set_power(10.0f); 


	// Load in shaders
	/*BASIC EFFECTS*/
	basicEff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	// Name of fragment shaders required
	vector<string> frag_shaders{"shaders/shader.frag", "shaders/part_direction.frag", "shaders/part_spot.frag", 
								"shaders/part_point.frag"};
	basicEff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	
	/*NORMAL MAPPING*/
	normalMappingEff.add_shader("shaders/shaderNormapMapping.vert", GL_VERTEX_SHADER); 
	vector<string> frag_shaders_normals{ "shaders/shaderNormalMapping.frag", "shaders/part_direction.frag", "shaders/part_spot.frag",
								 "shaders/part_point.frag", "shaders/part_normal_map.frag" };
	normalMappingEff.add_shader(frag_shaders_normals, GL_FRAGMENT_SHADER);
	 
	/*SHADOWS*/
	shadows_eff.add_shader("shaders/shadow.vert", GL_VERTEX_SHADER);
	vector<string> frag_shaders_shadows{ "shaders/shadow.frag", "shaders/part_direction.frag", "shaders/part_spot.frag",
		"shaders/part_point.frag", "shaders/part_shadow.frag" };
	shadows_eff.add_shader(frag_shaders_shadows, GL_FRAGMENT_SHADER);

	// Build effect
	basicEff.build();
	normalMappingEff.build();
	shadows_eff.build();

	// Set default camera properties 
	/*FREE CAMERA*/
	cams[1]->set_position(vec3(40.0f, 10.0f, 50.0f));
	cams[1]->set_target(vec3(0.0f, 0.0f, -100.0f));
	cams[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	return true;
}

bool update(float delta_time) {
	// Display the FPS
	cout << "FPS: " << 1.0f / delta_time << endl;
	if (1.0f / delta_time < 50) {
		cout << "Noooooooooooooooooooooooooooooooooooooooooooooooooo" << endl;
	}

	vec3 moonPos = vec3(0);
	vec3 diagonalMovement = vec3(0);

	// Set the correct camera index depending on the pressed button
	if (glfwGetKey(renderer::get_window(), 'F')) 
	{
		cameraIndex = 1;
	}

	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_1))
	{
		cameraIndex = 0;
		targetCamera = 1;
	}
	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_2))
	{
		cameraIndex = 0;
		targetCamera = 2;
	}
	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_3))
	{
		cameraIndex = 0;
		targetCamera = 3;
	}
	else if (glfwGetKey(renderer::get_window(), GLFW_KEY_4))
	{
		cameraIndex = 0;
		targetCamera = 4;
	}

	/*TARGET CAMERAS*/
	if (cameraIndex == 0)
	{
		if (targetCamera == 1) {
			cams[0]->set_position(vec3(50.0f, 50.0f, 50.0f));
			cams[0]->set_target(vec3(10.0f, 0.0f, 10.0f));
		}
		else if (targetCamera == 2) {
			cams[0]->set_position(vec3(-50.0f, 50.0f, 50.0f));
			cams[0]->set_target(vec3(10.0f, 0.0f, 10.0f));
		}
		else if (targetCamera == 3) {
			cams[0]->set_position(vec3(-10.0f, 100.0f, 0.0f));
			cams[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
		}
		else if (targetCamera == 4) {
			cams[0]->set_position(vec3(10.0f, 10.0f, 0.0f));
			cams[0]->set_target(vec3(-100.0f, 0.0f, 0.0f));
		}
		cams[0]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);
		cams[0]->update(delta_time);
	}
	/*FREE CAMERA*/
	else if (cameraIndex == 1)
	{
		double current_x = 0;
		double current_y = 0;
		double delta_x;
		double delta_y;
		vec3 pos = vec3(0);

		// The ratio of pixels to rotation
		static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
		static double ratio_height = (quarter_pi<float>() *
			(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
			static_cast<float>(renderer::get_screen_height());

		// Get the current cursor position
		glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

		// Calculate delta of cursor positions from last frame
		delta_x = current_x - cursor_x;
		delta_y = current_y - cursor_y;
		// Multiply deltas by ratios - gets actual change in orientation
		delta_x = delta_x * ratio_width;
		delta_y = delta_y * ratio_height;
		// Rotate camera by delta
		static_cast<free_camera*>(cams[1])->rotate(delta_x, -delta_y);

		// Use keyboard to move the camera
		if (glfwGetKey(renderer::get_window(), 'W')) {
			pos += vec3(0.0f, 0.0f, 20.0f) * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'S')) {
			pos -= vec3(0.0f, 0.0f, 20.0f) * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'A')) {
			pos -= vec3(20.0f, 0.0f, 0.0f) * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'D')) {
			pos += vec3(20.0f, 0.0f, 0.0f) * delta_time;
		}

		// Move camera
		static_cast<free_camera*>(cams[1])->move(pos);
		// Update the camera
		static_cast<free_camera*>(cams[1])->update(delta_time);

		// Update cursor pos
		cursor_x = current_x;
		cursor_y = current_y;
	}

	//shadow_geom["stick"].get_transform().rotate(vec3(quarter_pi<float>(),0.0f, 0.0f) * delta_time);

	// Rotate the Earth and the Moon around their Z axis
	normalMapMeshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	meshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);

	moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
	diagonalMovement = vec3(cos(velocity)*6.5, cos(velocity)*6.5, sin(velocity)*6.5);
	//Rotating moon around the Earth
	meshes["moon"].get_transform().position = moonPos + normalMapMeshes["earth"].get_transform().position;
	// Spot light in front of the wall
	spots[1].set_position(vec3(-35.5f, 10.5f, sin(velocity) * -40));
	meshes["torch"].get_transform().position = vec3(-25.5f, 10.5f, sin(velocity) * -40);
	// Day/night loop
	//dirLight.set_direction(vec3(0.0f, cos(velocity) * 5, sin(velocity)*5) + vec3(0, 0, 2));
	if (glfwGetKey(renderer::get_window(), 'L'))
	{
		// ambient intensity 
		dirLight.set_ambient_intensity(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		// Light colour
		dirLight.set_light_colour(vec4(0.0f, 0.0f, 0.0f, 1.0f)); 
	}
	else if (glfwGetKey(renderer::get_window(), 'M')){
		// ambient intensity 
		dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		// Light colour
		dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	velocity -= delta_time;

	// Update the shadow map properties from the spot light
	shadow.light_position = spots[1].get_position();
	shadow.light_dir = spots[1].get_direction();

	return true;
}

void renderNormalMapMesh()
{
	//mat4 ProjectionMat = perspective<float>(80.f, renderer::get_screen_aspect(), 0.1f, 1000.f);
	effect eff = normalMappingEff;
	renderer::bind(eff);

	// Bind direction lights
	renderer::bind(dirLight, "light");
	// Bind point lights
	renderer::bind(points, "points");
	// Bind spot lights 
	renderer::bind(spots, "spots");

	for (auto &item : normalMapMeshes) {
		// Gets the mesh
		auto geometryItem = item.second;
		// Gets the name of the mesh
		string geometryName = item.first;
		// Normal matrix
		auto N = geometryItem.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

		// Bind material
		renderer::bind(geometryItem.get_material(), "mat");

		// Bind texture to renderer
		//renderer::bind(textures[geometryName], 0);
		//renderer::bind(normal_maps[geometryName], 1);
		if (textures.find(geometryName) != textures.end())
		{
			renderer::bind(textures[geometryName], 0);
		}
		else
		{
			renderer::bind(textures["surroundings"], 0);
		}

		if (normal_maps.find(geometryName) != normal_maps.end())
		{
			renderer::bind(normal_maps[geometryName], 1);
		}
		else
		{
			renderer::bind(normal_maps["surroundings"], 1);
		}

		// Set the texture value for the shader here
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Set normal_map uniform        
		glUniform1i(eff.get_uniform_location("normal_map"), 1);

		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}
}

void renderMesh()
{
	//mat4 ProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);
	effect eff = basicEff;
	renderer::bind(eff);

	// Bind direction lights
	renderer::bind(dirLight, "light");
	// Bind point lights
	renderer::bind(points, "points");
	// Bind spot lights 
	renderer::bind(spots, "spots");

	for (auto &item : meshes) {
		// Gets the mesh
		auto geometryItem = item.second;
		// Gets the name of the mesh
		string geometryName = item.first;

		// Normal matrix
		auto N = geometryItem.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

		// Bind material
		renderer::bind(geometryItem.get_material(), "mat");

		// Bind texture to renderer
		if (textures.find(geometryName) != textures.end())
		{
			renderer::bind(textures[geometryName], 0);
		}
		else
		{
			renderer::bind(textures["surroundings"], 0);
		}

		// Set the texture value for the shader here
		glUniform1i(eff.get_uniform_location("tex"), 0);

		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}
}

void renderShadowMesh() 
{
	//mat4 ProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);
	// Set render target to shadow map
	renderer::set_render_target(shadow);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set render mode to cull face
	glCullFace(GL_FRONT);
	// *********************************
	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Bind shader
	renderer::bind(basicEff);

	// Render meshes
	for (auto &e : shadow_geom) {
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		// *********************************
		// View matrix taken from shadow map
		auto V = shadow.get_view();
		// *********************************

		//auto P = cams[cameraIndex]->get_projection();
		auto MVP = LightProjectionMat * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(basicEff.get_uniform_location("MVP"), // Location of uniform
			1,                                      // Number of values - 1 mat4
			GL_FALSE,                               // Transpose the matrix?
			value_ptr(MVP));                        // Pointer to matrix data
													// Render mesh
		renderer::render(m);
	}
	// Set render target back to the screen
	renderer::set_render_target();
	// Set cull face to back
	glCullFace(GL_BACK);

	// Bind shader
	renderer::bind(shadows_eff);

	// Render meshes
	for (auto &e : shadow_geom) {
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(shadows_eff.get_uniform_location("MVP"), // Location of uniform
			1,                                    // Number of values - 1 mat4
			GL_FALSE,                             // Transpose the matrix?
			value_ptr(MVP));                      // Pointer to matrix data
												  // Set M matrix uniform
		glUniformMatrix4fv(shadows_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform
		glUniformMatrix3fv(shadows_eff.get_uniform_location("N"), 1, GL_FALSE,
			value_ptr(m.get_transform().get_normal_matrix()));
		// *********************************
		// Set light transform
		auto lM = m.get_transform().get_transform_matrix();
		auto lV = shadow.get_view();
		//auto lP = cams[cameraIndex]->get_projection();
		auto lightMVP = LightProjectionMat * lV * lM;
		glUniformMatrix4fv(shadows_eff.get_uniform_location("lightMVP"), // Location of uniform
			1,                           			  // Number of values - 1 mat4
			GL_FALSE,								 // Transpose the matrix?
			value_ptr(lightMVP));
		// Bind material
		renderer::bind(m.get_material(), "mat");
		// Bind spot lights
		renderer::bind(spots[1], "spot");
		// Bind texture
		renderer::bind(textures["surroundings"], 0);
		// Set tex uniform
		glUniform1i(shadows_eff.get_uniform_location("tex"), 0);
		// Set eye position
		glUniform3fv(shadows_eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
		// Bind shadow map texture - use texture unit 1
		renderer::bind(shadow.buffer->get_depth(), 1);
		glUniform1i(shadows_eff.get_uniform_location("shadow_map"), 1);
		// Render mesh
		renderer::render(m);
		// *********************************
	}
}

bool render() {
	
	// Render meshes with normal maps
	renderNormalMapMesh();

	// Render meshes without normal maps
	renderMesh();

	// Render meshes with shadows
	renderShadowMesh();

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
}