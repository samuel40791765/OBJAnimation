	
// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo Material;
uniform sampler2D textureP;
out vec4 vFragColor;

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular

uniform float time;
uniform vec2 resolution = vec2(800., 600.);
uniform sampler2D ourTexture;
uniform float num;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 offset(ivec2 tile) {
	float r = rand(vec2(tile * 10));
	if (r < 5.0 / 15.0) return vec2(0.0);
	if (r < 10.0 / 15.0) return vec2(-1.0);
	if (r < 14.0 / 15.0) return vec2(1.0);
	return vec2(-0.2);
}

const vec3 orange = vec3(0.85, 0.35, 0.3);
const vec3 blue = vec3(0.12, 0.2, 0.3);
const vec3 ecru = vec3(0.8, 0.75, 0.68);
const vec3 dark = vec3(0.02, 0.02, 0.06);
const vec3 dark_blue = vec3(0.08, 0.1, 0.15);
const vec3 light = vec3(0.85, 0.85, 0.84);

vec3 rand_color (vec2 tile) {
	float r = rand(tile);
	if (r < 1.0 / 6.0) return orange;
	if (r < 2.0 / 6.0) return blue;
	if (r < 3.0 / 6.0) return ecru;
	if (r < 4.0 / 6.0) return dark_blue;
	if (r < 5.0 / 6.0) return dark;
	return light;
}

const float dimx = 16.0;
const float dimy = 9.0;

void main( void ) {
	vec4 textureColor = texture2D(textureP, UV);
	vec2 position = ( gl_FragCoord.xy / resolution.xy );
	ivec2 tile = ivec2(int(position.x * dimx), int(position.y * dimy));
	vec2 subpos = vec2(dimx * position.x - float(tile.x), dimy * position.y - float(tile.y));

	vec3 a = rand_color(2.34 * vec2(tile) + 5.2);
	vec3 b = rand_color(vec2(tile) * 2.78 + 7.8);
	
	vec3 k = mix(rand_color(16.5 * vec2(tile)), rand_color(9.5 * vec2(tile)), vec3(0.5 + 0.5 * sin(time)));
	vec3 u = mix(rand_color(2.4 * vec2(tile)), rand_color(5.8 * vec2(tile)), vec3(0.5 + 0.5 * cos(time)));
	
	float diff = length(subpos + sin(0.8 * time) * offset(tile) + 1.5 * cos(0.3 * time + 42.0 * vec2(tile)));
	vec3 value = diff > 1.0 ? k : u;
	
	vFragColor = vec4( vec3(value), 1.0 );
	vFragColor += textureColor;
}