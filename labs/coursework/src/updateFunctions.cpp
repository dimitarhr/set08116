/* updateFunctions.cpp
*  Consists of the functions that update the meshes and change between the cameras.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "updateFunctions.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void updateMeshesTransformations(float delta_time)
{
	// Using the cos and sin functions to calculate the range of movement
	// velocity is a global variable accumulated in the update function in 'main.cpp'
	vec3 moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
	vec3 levitatingRange = vec3(0.0f, cos(velocity)*1.5f, 0.0f);

	// Perform hierarchical transformations
	hierarchicalMesh[0].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	hierarchicalMesh[1].get_transform().rotate(vec3(quarter_pi<float>(), 0.0f, 0.0f) * delta_time);
	hierarchicalMesh[2].get_transform().rotate(vec3(0.0f, quarter_pi<float>(), 0.0f) * delta_time);
	hierarchicalMesh[3].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);
	hierarchicalMesh[4].get_transform().rotate(vec3(quarter_pi<float>(), 0.0f, 0.0f) * delta_time);

	// Rotate the Earth and the Moon around their Z axis
	normalMapMeshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, quarter_pi<float>()) * delta_time);
	basicMeshes["moon"].get_transform().rotate(vec3(0.0f, 0.0f, -quarter_pi<float>()) * delta_time);

	// Rotating moon around the Earth
	basicMeshes["moon"].get_transform().position = moonPos + normalMapMeshes["earth"].get_transform().position;

	// Move the spot light in front of the wall
	spots[1].set_position(vec3(-32.9f, 48.5f, sin(velocity) * -38+20));
	basicMeshes["torch"].get_transform().position = vec3(-38.0f, 48.5f, sin(velocity) * -38+20);

	// Levitating egg and protectors
	normalMapMeshes["dragonEgg"].get_transform().position = vec3(35.0f, 67.0f, -30.0f) + levitatingRange;
	hierarchicalMesh[0].get_transform().position = vec3(35.0f, 67.0f, -30.0f) + levitatingRange;

	// Move the water
	moveFactor += delta_time/15.0;
}

void checkPressedButton()
{
	// Free camera
	if (glfwGetKey(renderer::get_window(), 'F'))
	{
		cameraIndex = 1;
	}

	// Target cameras
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

	// Turn on/off the direct light
	if (glfwGetKey(renderer::get_window(), 'L'))
	{
		// Directional light OFF
		dirLight.set_ambient_intensity(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		dirLight.set_light_colour(vec4(0.4f, 0.2f, 0.2f, 1.0f));
	}
	else if (glfwGetKey(renderer::get_window(), 'K'))
	{
		// Directional light ON
		dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		dirLight.set_light_colour(vec4(0.99f, 0.37f, 0.33f, 1.0f));
	}

	int state = glfwGetMouseButton(renderer::get_window(), GLFW_MOUSE_BUTTON_RIGHT);
	// Zoom in
	if (state == GLFW_PRESS)
	{
		cams[cameraIndex]->set_projection(half_pi<float>() / 8.0, renderer::get_screen_aspect(), 0.1f, 1000.0f);
		screenMode = 1;
	}

	// Zoom out
	else
	{
		cams[cameraIndex]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
		screenMode = 0;
	}

	// Edge detection ON
	if (glfwGetKey(renderer::get_window(), 'E'))
	{
		if (sepia != 1 && motionBlur != 1)
		{
			edgeDetection = 1;
		}
	}

	// Edge detection OFF
	if (glfwGetKey(renderer::get_window(), 'R'))
	{
		edgeDetection = 0;
	}

	// Sepia ON
	if (glfwGetKey(renderer::get_window(), 'Z'))
	{
		if (edgeDetection != 1 && motionBlur != 1)
		{
			sepia = 1;
		}
	}

	// Sepia OFF
	if (glfwGetKey(renderer::get_window(), 'X'))
	{
		sepia = 0;
	}
	// Motion Blur ON
	if (glfwGetKey(renderer::get_window(), 'M'))
	{
		if (edgeDetection != 1 && sepia != 1)
		{
			motionBlur = 1;
		}
	}

	// Motion Blur OFF
	if (glfwGetKey(renderer::get_window(), 'N'))
	{
		motionBlur = 0;
	}

	// Wireframe ON
	if (glfwGetKey(renderer::get_window(), 'F'))
	{
		wireFrame = 1;
	}

	// Wireframe OFF
	if (glfwGetKey(renderer::get_window(), 'G'))
	{
		wireFrame = 0;
	}
}