#version 330 core
// Inputs
in vec3 ourColor;
in vec2 TexCoord;
// Outputs
out vec4 FragColor;
// Uniforms
uniform sampler2D texture1;

// Main function
void main() {
	FragColor = texture(texture1, TexCoord);
}