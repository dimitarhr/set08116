#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

effect eff;
//target_camera targetCam;
//free_camera freeCamera;
std::array<camera*, 2> cams;
map<string, mesh> meshes;
texture surface, earth, lavaRing, disturb, moonSurface, box;
mesh spikyBall;
directional_light dirLight;
vector<spot_light> spots(5);

double cursor_x = 0.0;
double cursor_y = 0.0;
double angleLuna = 0.0f;
double newX, newY;

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
	spikyBall = mesh(geometry_builder::create_pyramid());

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
	
	spikyBall.get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
	spikyBall.get_transform().translate(vec3(-20.0f, 2.0f, 30.0f));
	spikyBall.get_transform().rotate(vec3(0.0f, 0.0f, pi<float>()));

	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25
	// Earth
	material objectMaterial;
	objectMaterial.set_emissive(vec4(0.0f,0.0f,0.0f,1.0f));
	objectMaterial.set_specular(vec4(0.5f, 0.5f, 0.5f, 0.5f));
	objectMaterial.set_shininess(35);
	objectMaterial.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["earth"].set_material(objectMaterial);

	// Moon
	meshes["moon"].set_material(objectMaterial);

	// Ring
	objectMaterial.set_diffuse(vec4(0.8f, 0.8f, 0.8f, 1.0f));
	meshes["ring"].set_material(objectMaterial);

	// ringBase
	meshes["ringBase"].set_material(objectMaterial);

	meshes["stickBoxLeft"].set_material(objectMaterial);
	meshes["smallStickBoxLeft"].set_material(objectMaterial);
	meshes["stickBoxRight"].set_material(objectMaterial);
	meshes["smallStickBoxRight"].set_material(objectMaterial);
	meshes["stickBoxBack"].set_material(objectMaterial);
	meshes["smallStickBoxBack"].set_material(objectMaterial);
	meshes["stickBoxFront"].set_material(objectMaterial);
	meshes["smallStickBoxFront"].set_material(objectMaterial);
	meshes["sphereLeft"].set_material(objectMaterial);

	// spikyBall
	spikyBall.set_material(objectMaterial);

	// Load texture
	surface = texture("textures/moon_surface.jpg",true,true);
	earth = texture("textures/earth.jpg", true, true);
	lavaRing = texture("textures/lavatile.jpg", true, true);
	disturb = texture("textures/disturb.jpg", true, true);
	moonSurface = texture("textures/moon_sphere.jpg", true, true);
	box = texture("textures/moon_surface.png", true, true);

	// ambient intensity (0.3, 0.3, 0.3)  
	dirLight.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	// Light colour white
	dirLight.set_light_colour(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	// Light direction (1.0, 1.0, -1.0)
	dirLight.set_direction(vec3(1.0f, 1.0f, 2.0f));
	//25 12 10
	spots[0].set_position(vec3(25, 10, 15));
	spots[0].set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(1, -1, -1)));
	spots[0].set_range(100);
	spots[0].set_power(0.5f);

	// Load in shaders
	eff.add_shader("shaders/shader.vert", GL_VERTEX_SHADER);
	// Name of fragment shaders required
	vector<string> frag_shaders{"shaders/shader.frag", "shaders/part_direction.frag", "shaders/part_spot.frag" };
	eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
	
	// Build effect
	eff.build();

	// Set camera properties

/*TARGET CAMERA*/
	/*cam.set_position(vec3(0.0f, 10.0f, 50.0f));
	cam.set_target(vec3(0.0f, 0.0f, -100.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);*/

/*FREE CAMERA*/
	cams[1]->set_position(vec3(40.0f, 10.0f, 50.0f));
	cams[1]->set_target(vec3(0.0f, 0.0f, -100.0f));
	cams[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

	return true;
}


bool update(float delta_time) {
	cout << "FPS: " << 1.0f / delta_time << endl;
	// Update the camera

/*TARGET CAMERA*/
	/*cam.update(delta_time);*/

/*FREE CAMERA*/
	double current_x = 0;
	double current_y = 0;
	double delta_x;
	double delta_y;
	vec3 pos = vec3(0);
	vec3 lunaPos = vec3(0);

	// The ratio of pixels to rotation - remember the fov
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
	// Rotate cameras by delta
	static_cast<free_camera*>(cams[1])->rotate(delta_x, -delta_y);

	// Use keyboard to move the camera - WSAD
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

	// Rotate the sphere
	meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	meshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	spikyBall.get_transform().rotate(vec3(0.0f, half_pi<float>() * delta_time, 0.0f));
	//Rotating moon around the Earth
	lunaPos = vec3(cos(angleLuna)*4.5f, 0.0f, sin(angleLuna)*4.5f);
	meshes["moon"].get_transform().position = lunaPos + meshes["earth"].get_transform().position;
	angleLuna -= 1.0 * delta_time;

	return true;
}

bool render() {

	// Bind texture to renderer
	renderer::bind(surface,0);
	renderer::bind(earth, 1);
	renderer::bind(lavaRing, 2);
	renderer::bind(disturb, 3);
	renderer::bind(moonSurface, 4);
	renderer::bind(box, 5);

	// Bind effect
	renderer::bind(eff);

	for (auto &item : meshes) {
		auto geometryItem = item.second;
		// Normal matrix
		auto N = geometryItem.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = cams[1]->get_view();
		auto P = cams[1]->get_projection();
		auto MVP = P * V * M;
		
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		
		// Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		
		// Set N matrix uniform
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

		// Bind material
		renderer::bind(geometryItem.get_material(), "mat");
		
		// Bind point lights
		renderer::bind(dirLight, "light");

		// Bind spot lights 
		renderer::bind(spots, "spots"); 

		// Set the texture value for the shader here
		if (item.first == "earth") 
		{
			glUniform1i(eff.get_uniform_location("tex"), 1);
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
			glUniform1i(eff.get_uniform_location("tex"), 0);
		}
		else
		{
			glUniform1i(eff.get_uniform_location("tex"), 5);
		}

		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[1]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}

/*SPIKY BALL*/
	// Create MVP matrix
	/*auto M = spikyBall.get_transform().get_transform_matrix();
	auto V = freeCamera.get_view();
	auto P = freeCamera.get_projection();
	auto MVP = P * V * M;

	// Bottom
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);

	// Right
	MVP = P * V * rotate(translate(M, vec3(-1.0f, -1.0f, 0.0f)), half_pi<float>(), vec3(0, 0, 1));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);

	// Left
	MVP = P * V * rotate(translate(M, vec3(1.0f, -1.0f, 0.0f)), -half_pi<float>(), vec3(0, 0, 1));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);

	// Top
	MVP = P * V * rotate(translate(M, vec3(0.0f, -2.0f, 0.0f)), pi<float>(), vec3(0, 0, 1));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);

	// Front
	MVP = P * V * rotate(translate(M, vec3(0.0f, -1.0f, 1.0f)), half_pi<float>(), vec3(1, 0, 0));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);

	// Back
	MVP = P * V * rotate(translate(M, vec3(0.0f, -1.0f, -1.0f)), -half_pi<float>(), vec3(1, 0, 0));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1i(eff.get_uniform_location("tex"), 3);
	renderer::render(spikyBall);
	*/
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