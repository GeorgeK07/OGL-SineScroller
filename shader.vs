#version 330 core
// Inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in vec3 aOffset;
// Outputs
out vec3 ourColor;
out vec2 TexCoord;
// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float xTexOffset;
uniform float yTexOffset;

// Main function
void main() {
	//	Multiply vertex vectors by 3d space transformation matrices (matrix
	// multiplication done from right to left)
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	// Set ourColor output var to input aTexCoord from vertex attrib array
	TexCoord = vec2(aTexCoord.x + xTexOffset, aTexCoord.y + yTexOffset);
	//TexCoord = aTexCoord;
}