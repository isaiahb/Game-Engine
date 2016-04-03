#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;
//layout (location = 3) in mat4 instanceMatrix;


uniform mat4 instanceMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 scale;
uniform vec3 color;

out DATA
{
    vec4 position;
    vec4 color;
    vec4 ambient;
    vec3 normal;
    vec2 texCoord;

} fragment;

// Vertex Shader //
void main()
{
	vec2 tc = vec2(texCoord);
    mat4 fullMatrix = projectionMatrix * viewMatrix * instanceMatrix;
    gl_Position = fullMatrix * position;
    vec3 norm = mat3(transpose(inverse(instanceMatrix))) * vec3(normal);
 
	if (norm.x > 0 || norm.x < 0) {
		tc.x *= scale.z;
		tc.y *= scale.y;
	}
	if (norm.y > 0 || norm.y < 0) {
		tc.x *= scale.x;
		tc.y *= scale.z;
	}
	if (norm.z > 0 || norm.z < 0) {
		tc.x *= scale.x;
		tc.y *= scale.y;
	}
	
	fragment.position = instanceMatrix * position;
	fragment.ambient = vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f;
	fragment.color = vec4(color, 1);
	fragment.normal = norm;
	fragment.texCoord = tc;// * mat2(scale);

}
