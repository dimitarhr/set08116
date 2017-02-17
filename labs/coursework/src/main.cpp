#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
//target_camera cam;
free_camera freeCamera;
map<string, mesh> meshes;
texture surface, earth, lavaRing, disturb, moonSurface, box;
mesh spikyBall;

double cursor_x = 0.0;
double cursor_y = 0.0;
double angleLuna = 0.0f;
double newX, newY;

// before load_content
bool initialise() {
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

bool load_content() {
	// Create meshes
	meshes["floorPlane"] = mesh(geometry_builder::create_plane());
	meshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 5.0f));
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
	meshes["earth"].get_transform().translate(vec3(25.0f, 10.0f, 10.0f));
	meshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["moon"].get_transform().scale = vec3(0.9f, 0.9f, 0.9f);
	meshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
	meshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	meshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	meshes["ring"].get_transform().translate(vec3(25.0f, 10.0f, 10.0f));
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

	// Load texture
	surface = texture("textures/moon_surface.jpg",true,true);
	earth = texture("textures/earth.jpg", true, true);
	lavaRing = texture("textures/lavatile.jpg", true, true);
	disturb = texture("textures/disturb.jpg", true, true);
	moonSurface = texture("textures/moon_sphere.jpg", true, true);
	box = texture("textures/moon_surface.png", true, true);

	// Load in shaders
	eff.add_shader("shaders/basic_textured.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/basic_textured.frag", GL_FRAGMENT_SHADER);
	
	// Build effect
	eff.build();

	// Set camera properties

/*TARGET CAMERA*/
	/*cam.set_position(vec3(0.0f, 10.0f, 50.0f));
	cam.set_target(vec3(0.0f, 0.0f, -100.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);*/

/*FREE CAMERA*/
	freeCamera.set_position(vec3(40.0f, 10.0f, 50.0f));
	freeCamera.set_target(vec3(0.0f, 0.0f, -100.0f));
	freeCamera.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);

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
	freeCamera.rotate(delta_x, -delta_y);

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
	freeCamera.move(pos);
	// Update the camera
	freeCamera.update(delta_time);

	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;

	// Rotate the sphere
	meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	meshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	spikyBall.get_transform().rotate(vec3(0.0f, half_pi<float>() * delta_time, 0.0f));
	// Not working
	lunaPos = vec3((cos(angleLuna)*4.5f), 0.0f, (sin(angleLuna)*4.5f));
	//meshes["moon"].get_transform().translate(vec3(25.0f, 10.0f, 18.0f));
	//meshes["earth"].get_transform().translate(vec3(25.0f, 10.0f, 10.0f));
	/*newX = cos(angleLuna)*(25 - 25) - sin(angleLuna)*(18 - 10) + 25;
	newY = sin(angleLuna)*(25 - 25) + cos(angleLuna)*(18 - 10) + 10;
	lunaPos = vec3(newX, 0.0f, newY);*/
	meshes["moon"].get_transform().position += lunaPos*delta_time;
	angleLuna -= 0.01f;

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
		
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = freeCamera.get_view();
		auto P = freeCamera.get_projection();
		auto MVP = P * V * M;
		
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		
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

		// Render geometry
		renderer::render(geometryItem);
	}

/*SPIKY BALL*/
	// Create MVP matrix
	auto M = spikyBall.get_transform().get_transform_matrix();
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