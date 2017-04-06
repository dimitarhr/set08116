/* setCamera.cpp
*  Consists of the functions that are required to set the different cameras.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "setCamera.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void setTargetCamera(float delta_time)
{
	if (targetCamera == 1) {
		cams[0]->set_position(vec3(60.0f, 45.0f, 50.0f));
		cams[0]->set_target(vec3(40.0f, 50.0f, 10.0f));
	}
	else if (targetCamera == 2) {
		cams[0]->set_position(vec3(-40.0f, 45.0f, 50.0f));
		cams[0]->set_target(vec3(40.0f, 40.0f, 10.0f));
	}
	else if (targetCamera == 3) {
		cams[0]->set_position(vec3(10.0f, 500.0f, 0.0f));
		cams[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
	}
	else if (targetCamera == 4) {
		cams[0]->set_position(vec3(10.0f, 100.0f, 20.0f));
		cams[0]->set_target(vec3(-100.0f, 0.0f, 10.0f));
	}
	if (screenMode == 1)
	{
		cams[0]->set_projection(half_pi<float>() / 8.0, renderer::get_screen_aspect(), 2.414f, 1000.0f);
	}
	else
	{
		cams[0]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	}
}

void setFreeCamera(float delta_time)
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
		pos += vec3(0.0f, 0.0f, 40.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		pos -= vec3(0.0f, 0.0f, 40.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		pos -= vec3(40.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		pos += vec3(40.0f, 0.0f, 0.0f) * delta_time;
	}

	// Move camera
	static_cast<free_camera*>(cams[1])->move(pos);

	// Update cursor positions
	cursor_x = current_x;
	cursor_y = current_y;

	if (screenMode == 1)
	{
		cams[1]->set_projection(half_pi<float>() / 8.0, renderer::get_screen_aspect(), 2.414f, 1000.0f);
	}
	else
	{
		cams[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	}
}
