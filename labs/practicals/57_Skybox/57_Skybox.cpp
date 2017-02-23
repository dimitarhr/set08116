#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;
//target_camera cam;
free_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;

// before load_content
bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}
  
bool load_content() {
  // Create a sphere
  sphere = mesh(geometry_builder::create_sphere(25, 25));
  // *********************************
  // Create box geometry for skybox
  skybox = mesh(geometry_builder::create_box());
  
  // Scale box by 100
  skybox.get_transform().scale *= vec3(100);
  
  // Load the cubemap
  array<string, 6> filenames = { "textures/sky_front.png", "textures/sky_back.png", "textures/sky_top.png",
	  "textures/sky_botton.png", "textures/sky_left.png", "textures/sky_right.png" };
  // Create cube_map
  cube_map = cubemap(filenames); 
   
  // *********************************
  // Load in shaders
  eff.add_shader("57_Skybox/shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("57_Skybox/shader.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // *********************************
  // Load in skybox effect
  sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
  sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
  // Build effect
  sky_eff.build();
  // *********************************

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}
float theta;
bool update(float delta_time) {
  /*theta += pi<float>() * delta_time;
  cam.set_position(rotate(vec3(15.0f, 5.0f, 15.0f), theta * 0.05f, vec3(0, 1.0f, 0)));
  cam.update(delta_time);*/
  // The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x = 0;
	double current_y = 0;
	double delta_x;
	double delta_y;
	vec3 pos = vec3(0);
	// *********************************
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	delta_x = current_x - cursor_x;
	delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;

	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	cam.rotate(delta_x, -delta_y);
	// Use keyboard to move the camera - WSAD
	if (glfwGetKey(renderer::get_window(), 'W')) {
		pos += vec3(0.0f, 0.0f, 20.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		pos += vec3(0.0f, 0.0f, -20.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		pos += vec3(-20.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		pos += vec3(20.0f, 0.0f, 0.0f) * delta_time;
	}
	// Move camera
	cam.move(pos);
	// Update the camera
	cam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
  // *********************************
  // Set skybox position to camera position (camera in centre of skybox)
  skybox.get_transform().position = cam.get_position();
  // *********************************
  return true;
}

bool render() {
  // *********************************
  // Disable depth test,depth mask,face culling
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	//glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
  // Bind skybox effect
	renderer::bind(sky_eff);
  // Calculate MVP for the skybox
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;
	renderer::bind(cube_map, 0);
  // Set MVP matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Set cubemap uniform
	glUniform1f(eff.get_uniform_location("cubemap"), 0);

  // Render skybox
	renderer::render(skybox);
  // Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

  // *********************************

  // Bind effect
  /*renderer::bind(eff);
  // Create MVP matrix
  M = sphere.get_transform().get_transform_matrix();
  V = cam.get_view();
  P = cam.get_projection();
  MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Render mesh
  renderer::render(sphere);*/

  return true;
}

void main() {
  // Create application
  app application("57_Skybox");
  // Set methods
  application.set_load_content(load_content);
  application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}