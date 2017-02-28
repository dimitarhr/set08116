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
	vec3 moonPos = vec3(cos(velocity)*4.5f, 0.0f, sin(velocity)*4.5f);
	vec3 levitatingRange = vec3(0.0f, cos(velocity)*1.5f, 0.0f);

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
	spots[1].set_position(vec3(-42.9f, 5.5f, sin(velocity) * -40));
	basicMeshes["torch"].get_transform().position = vec3(-38.0f, 5.5f, sin(velocity) * -40);

	// Levitating egg and protectors
	normalMapMeshes["dragonEgg"].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;
	hierarchicalMesh[0].get_transform().position = vec3(25.0f, 12.0f, -30.0f) + levitatingRange;
}

void checkPressedButton()
{
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

	// Turn on/off the direct light
	if (glfwGetKey(renderer::get_window(), 'L'))
	{
		// Directional light OFF
		dirLight.set_ambient_intensity(vec4(0.0f, 0.0f, 0.0f, 1.0f));
		dirLight.set_light_colour(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (glfwGetKey(renderer::get_window(), 'M'))
	{
		// Directional light ON
		dirLight.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		dirLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}