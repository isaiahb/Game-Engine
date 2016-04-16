#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 5) out;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix = mat4(1);


void main() {
	mat4 fullmatrix = projectionMatrix * viewMatrix  * modelMatrix;
	//Top Left
	vec4 topLeft = fullmatrix * (gl_in[0].gl_Position + vec4(-1, 0, -1, 1));
	vec4 topRight = fullmatrix * (gl_in[0].gl_Position + vec4(1, 0, -1, 1));
	vec4 bottomLeft = fullmatrix * (gl_in[0].gl_Position + vec4(-1, 0, 1, 1));
	vec4 bottomRight = fullmatrix * (gl_in[0].gl_Position + vec4(1, 0, 1, 1));

	gl_Position =  topLeft;
	EmitVertex();
	
	gl_Position = topRight;
	EmitVertex();
	
	gl_Position = bottomRight;
	EmitVertex();
	
	gl_Position = bottomLeft;
	EmitVertex();
	
	gl_Position = topLeft;
	EmitVertex();


	EndPrimitive();
}