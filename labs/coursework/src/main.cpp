#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

effect eff;
std::array<camera*, 2> cams;
int cameraIndex = 1;
int targetCamera = 1;
map<string, mesh> meshes;
map<string, texture> textures;
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
	// Create meshes
	meshes["floorPlane"] = mesh(geometry_builder::create_plane());
	meshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 6.5f));
	meshes["earth"] = mesh(geometry_builder::create_sphere(60, 60));
	meshes["moon"] = mesh(geometry_builder::create_sphere(30, 30));
	meshes["ringBase"] = mesh(geometry_builder::create_pyramid());
	meshes["stickBoxLeft"] = mesh(geometry_builder::create_box(vec3(0.2f,3.0f,0.2f)));
	meshes["smallStickBoxLeft"] = mesh(geometry_builder::create_box(vec3(5.0f, 5.0f, 5.0f)));
	meshes["stickBoxRight"] = mesh(geometry_builder::create_box(vec3(0.5f, 2.0f, 0.2f)));
	meshes["smallStickBoxRight"] = mesh(geometry_builder::create_box(vec3(4.0f, 4.0f, 4.0f)));
	meshes["stickBoxBack"] = mesh(geometry_builder::create_box(vec3(0.2f, 4.0f, 0.2f)));
	meshes["smallStickBoxBack"] = mesh(geometry_builder::create_box(vec3(4.5f, 4.5f, 4.5f)));
	meshes["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(0.5f, 1.0f, 0.5f)));
	meshes["smallStickBoxFront"] = mesh(geometry_builder::create_box(vec3(3.0f, 3.0f, 3.0f)));
	meshes["sphereLeft"] = mesh(geometry_builder::create_sphere(30, 30));
	// Under testing
	meshes["wall"] = mesh(geometry_builder::create_box(vec3(1,7,32)));
	meshes["torch"] = mesh(geometry_builder::create_cylinder());
	meshes["whiteHouse"] = mesh(geometry_builder::create_cylinder());
	meshes["sun"] = mesh(geometry_builder::create_sphere(30, 30));
	meshes["smallTorus"] = mesh(geometry_builder::create_torus(20,20,1,2.5f));
	meshes["mediumTorus"] = mesh(geometry_builder::create_torus(30, 30, 1,5));
	meshes["largeTorus"] = mesh(geometry_builder::create_torus(40, 40,1,10));

	// Transform objects
	meshes["earth"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	meshes["earth"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	meshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["moon"].get_transform().scale = vec3(0.9f, 0.9f, 0.9f);
	meshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
	meshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	meshes["ring"].get_transform().translate(vec3(25.0f, 12.0f, 10.0f));
	meshes["ringBase"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["ringBase"].get_transform().translate(vec3(25.0f, 2.5f, 10.0f));
	meshes["stickBoxLeft"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxLeft"].get_transform().translate(vec3(13.0f, 7.5f, 10.0f));
	meshes["smallStickBoxLeft"].get_transform().translate(vec3(13.0f, 2.5f, 5.0f));
	meshes["stickBoxRight"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxRight"].get_transform().translate(vec3(40.0f, 5.0f, 10.0f));
	meshes["smallStickBoxRight"].get_transform().translate(vec3(40.0f, 2.0f, 5.0f));
	meshes["stickBoxBack"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxBack"].get_transform().translate(vec3(25.0f, 9.5f, 0.0f));
	meshes["smallStickBoxBack"].get_transform().translate(vec3(21.0f, 2.0f, 0.0f));
	meshes["stickBoxFront"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["stickBoxFront"].get_transform().translate(vec3(22.0f, 2.5f, 25.0f));
	meshes["smallStickBoxFront"].get_transform().translate(vec3(17.0f, 1.5f, 25.0f));
	meshes["sphereLeft"].get_transform().translate(vec3(15.0f, 3.0f, 17.0f));
	meshes["sphereLeft"].get_transform().scale = vec3(3.0f, 3.0f, 3.0f);
	// Under testing
	meshes["wall"].get_transform().scale = vec3(3.0f, 3.0f, 3.0f);
	meshes["wall"].get_transform().translate(vec3(-48.5f, 10.5f, 0.0f));
	meshes["torch"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
	meshes["torch"].get_transform().translate(vec3(-15.5f, 10.5f, -40));
	meshes["whiteHouse"].get_transform().translate(vec3(10, 0, -8));
	meshes["smallTorus"].get_transform().translate(vec3(25.0f, 12.0f, -20.0f));
	meshes["mediumTorus"].get_transform().translate(vec3(25.0f, 12.0f, -20.0f));
	meshes["largeTorus"].get_transform().translate(vec3(25.0f, 12.0f, -20.0f));

	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25 
	// Earth
	material objectMaterial;

	// Floor
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//objectMaterial.set_specular(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	objectMaterial.set_shininess(35);
	objectMaterial.set_diffuse(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	meshes["floorPlane"].set_material(objectMaterial);

	// Torch
	objectMaterial.set_emissive(vec4(0.5f, 0.5f, 0.0f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.0f, 1.0f));
	meshes["torch"].set_material(objectMaterial);

	// Earth
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["earth"].set_material(objectMaterial);

	objectMaterial.set_specular(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	meshes["wall"].set_material(objectMaterial);

	// Moon 
	meshes["moon"].set_material(objectMaterial);

	// Ring
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["ring"].set_material(objectMaterial);

	objectMaterial.set_diffuse(vec4(0.3f, 0.3f, 0.3f, 1.0f)); 

	// ringBase
	objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	objectMaterial.set_shininess(35);
	objectMaterial.set_diffuse(vec4(0.3f, 0.3f, 0.3f, 1.0f));

	meshes["ringBase"].set_material(objectMaterial);
	
	// The rest of the surroundings
	objectMaterial.set_shininess(65);
	meshes["stickBoxLeft"].set_material(objectMaterial);
	meshes["smallStickBoxLeft"].set_material(objectMaterial);
	meshes["stickBoxRight"].set_material(objectMaterial);
	meshes["smallStickBoxRight"].set_material(objectMaterial);
	meshes["stickBoxBack"].set_material(objectMaterial);
	meshes["smallStickBoxBack"].set_material(objectMaterial);
	meshes["stickBoxFront"].set_material(objectMaterial);
	meshes["smallStickBoxFront"].set_material(objectMaterial);
	meshes["sphereLeft"].set_material(objectMaterial);

	// Load texture 
	textures["surface"] = texture("textures/sand.jpg",true,true);
	textures["earth"] = texture("textures/earth.jpg", true, true);
	textures["lavaRing"] = texture("textures/lavatile.jpg", true, true);
	textures["disturb"] = texture("textures/disturb.jpg", true, true);
	textures["moonSurface"] = texture("textures/moon_sphere.jpg", true, true);
	textures["box"] = texture("textures/red_rock.jpg", true, true);
	textures["earth_normal_map"] = texture("textures/earth_normalmap.jpg", true, true);
	textures["rocks_normal_map"] = texture("textures/rock_norma_map.jpg", true, true);
	textures["sand_normal-map"] = texture("textures/sand_normal-map.jpg", true, true);

	/*DIRECTIONAL LIGHT*/
	// ambient intensity 
	dirLight.set_ambient_intensity(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	// Light colour
	dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	// Light direction
	dirLight.set_direction(vec3(1.0f, 1.0f, 1.0f));

	/*points[0].set_position(vec3(0, 0, -10));
	points[0].set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	points[0].set_range(200.0f);*/
	
	/*SPOT LIGHT*/
	// Spot light in the torrus
	spots[0].set_position(vec3(25.0f, 0.0f, 10.0f));
	spots[0].set_light_colour(vec4(1.0f, 0.3f, 0.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(0, 1, -1)));
	spots[0].set_range(80);
	spots[0].set_power(0.5f);

	// Spot light in front of the wall
	spots[1].set_position(vec3(-5.5f, 10.5f, -40));
	spots[1].set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	spots[1].set_direction(normalize(vec3(-1,0, 0)));
	spots[1].set_range(100);
	spots[1].set_power(0.5f);
	
	// The green spot light
	spots[2].set_position(vec3(10, 0, -8));
	spots[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	spots[2].set_direction(normalize(vec3(1, 0, 1)));
	spots[2].set_range(50);
	spots[2].set_power(0.1f);


	// Load in shaders
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	// Name of fragment shaders required
	vector<string> frag_shaders{"shaders/shader.frag", "shaders/part_direction.frag", "shaders/part_spot.frag", "shaders/part_point.frag", "shaders/part_normal_map.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	
	// Build effect
	eff.build();

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


	if (cameraIndex == 0)
	{
	/*TARGET CAMERAS*/
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
		cams[0]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);
		cams[0]->update(delta_time);
	}
	else if (cameraIndex == 1)
	{
	/*FREE CAMERA*/
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

	// Rotate the Earth and the Moon around their Z axis
	meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	meshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	meshes["smallTorus"].get_transform().rotate(vec3(sin(delta_time)*-0.01, 0.0f, cos(delta_time)*-0.01));
	meshes["mediumTorus"].get_transform().rotate(vec3(cos(delta_time)*0.01, 0.0f, sin(delta_time)*0.01));
	meshes["largeTorus"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);

	moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
	diagonalMovement = vec3(cos(velocity)*6.5, cos(velocity)*6.5, sin(velocity)*6.5);
	//Rotating moon around the Earth
	meshes["moon"].get_transform().position = moonPos + meshes["earth"].get_transform().position;
	// Spot light in front of the wall
	spots[1].set_position(vec3(-5.5f, 10.5f, sin(velocity) * -40));
	meshes["torch"].get_transform().position = vec3(-5.5f, 10.5f, sin(velocity) * -40);
	// Lights around the Earth
	spots[2].set_position(moonPos*vec3(2));
	// Object moving in diagonals around the Earth
	meshes["whiteHouse"].get_transform().position = diagonalMovement + meshes["mediumTorus"].get_transform().position;
	// Day/night loop
	//dirLight.set_direction(vec3(0.0f, cos(velocity) * 5, sin(velocity)*5) + vec3(0, 0, 2));
	//meshes["sun"].get_transform().position = vec3(0.0f, cos(velocity) * 5, sin(velocity)*5) + vec3(1, 1, 2);
	if (glfwGetKey(renderer::get_window(), 'L'))
	{
		// ambient intensity 
		dirLight.set_ambient_intensity(vec4(0.2f, 0.2f, 0.2f, 1.0f));
		// Light colour
		dirLight.set_light_colour(vec4(0.2f, 0.2f, 0.2f, 1.0f)); 
	}
	else if (glfwGetKey(renderer::get_window(), 'M')){
		// ambient intensity 
		dirLight.set_ambient_intensity(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		// Light colour
		dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	velocity -= delta_time;

	return true;
}

bool render() {
	// Bind texture to renderer
	renderer::bind(textures["surface"],0);
	renderer::bind(textures["earth"], 1);
	renderer::bind(textures["lavaRing"], 2);
	renderer::bind(textures["disturb"], 3);
	renderer::bind(textures["moonSurface"], 4);
	renderer::bind(textures["box"], 5);
	// Bind normal_map
	renderer::bind(textures["earth_normal_map"], 6);
	renderer::bind(textures["rocks_normal_map"], 7);
	renderer::bind(textures["sand_normal-map"], 8);
	 
	// Bind effect
	renderer::bind(eff);
	 

	// Bind direction lights
	renderer::bind(dirLight, "light");
	// Bind point lights
	renderer::bind(points, "points");
	// Bind spot lights 
	renderer::bind(spots, "spots");

	for (auto &item : meshes) {
		auto geometryItem = item.second;
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

		// Set the texture value for the shader here
		if (item.first == "earth") 
		{
			glUniform1i(eff.get_uniform_location("tex"), 1);
			  
			// Set normal_map uniform        
			glUniform1i(eff.get_uniform_location("normal_map"), 6);
		}
		else if (item.first == "ring")
		{
			glUniform1i(eff.get_uniform_location("tex"), 2);
		}
		else if (item.first == "ringBase")
		{
			glUniform1i(eff.get_uniform_location("tex"), 3);
		}
		else if (item.first == "moon")
		{
			glUniform1i(eff.get_uniform_location("tex"), 4);
		}
		else if (item.first == "floorPlane")
		{
			glUniform1i(eff.get_uniform_location("normal_map"), 8);
			glUniform1i(eff.get_uniform_location("tex"), 0);
		}
		else
		{
			glUniform1i(eff.get_uniform_location("normal_map"), 7); 
			glUniform1i(eff.get_uniform_location("tex"), 5); 
		}

		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}
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