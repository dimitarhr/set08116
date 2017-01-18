#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>

using namespace std;
using namespace glm;

int main() {

	vec2 first (1.0f, 0.0f);
	vec3 second (0.0f, 0.0f, 1.0f);
	vec4 third(0.0f, 1.0f, 0.2f,9.0f);
	vec4 fourth(8.0f, 1.0f, 2.0f, 9.0f);
	vec4 sum = third + fourth;
	double magnitude = length(sum);
	vec4 normVector = normalize(fourth);

	///////////////////////////////////
	// Testing Projection

	vec2 a(3, 3);
	vec2 b(5, 0);
	vec2 result = proj(a,b);

	//////////////////////////////////

	mat4 m(1.0f);

	cout << m[0][0] << endl;

	cout << first.x<<", "<<first.y<<endl;
	cout << second.x << ", " << second.y << ", "<<second.z<<endl;
	cout << third.x << ", " << third.y << ", " << third.z << ", "<<third.w<<endl;
	cout << fourth.x << ", " << fourth.y << ", " << fourth.z << ", " << fourth.w << endl;
	cout << sum.x << ", " << sum.y << ", " << sum.z << ", " << sum.w << endl;
	cout << magnitude<<endl;
	cout << normVector.x << ", " << normVector.y << ", " << normVector.z << ", " << normVector.w << endl;
	cout << "Projection of a onto b: "<<result.x << ", " << result.y << endl;
	return 0;
}