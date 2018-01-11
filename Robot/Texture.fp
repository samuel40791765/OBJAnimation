// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430

in vec2 UV;

out vec3 color;

uniform sampler2D Texture;

void main()
{
	color = texture(Texture,UV).rgba;
}