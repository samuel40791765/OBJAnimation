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

#define MAX_ITER 9 // water depth

uniform float time;
const vec2 iResolution = vec2(800., 600.); 

float field(in vec3 p) {
	float strength = 7. + .03 * log(1.e-6 + fract(4373.11));
	float accum = 0.;
	float prev = 0.;
	float tw = 0.;
	for (int i = 0; i < 32; ++i) {
	float mag = dot(p, p);
	p = abs(p) / mag + vec3(-.5, -.4, -1.5);
	float w = exp(-float(i) / 7.);
	accum += w * exp(-strength * pow(abs(mag - prev), 2.33));
	tw += w;  prev = mag;
	}
	return max(0., 5. * accum / tw - .7);
}

void main() {
	vec4 textureColor = texture2D(textureP, UV);
	vec2 uv = 2. * gl_FragCoord.xy / iResolution.xy - 1.;
	vec2 uvs = uv * iResolution.xy / max(iResolution.x, iResolution.y);
	vec3 p = vec3(uvs / 4., 0) + vec3(1., -1.3, 0.);
	p += .2 * vec3(sin(time / 16.), sin(time / 12.), sin(time / 128.));
	float t = field(p);
	float v = (1. - exp((abs(uv.x) - 1.) * 6.)) * (1. - exp((abs(uv.y) - 1.) * 6.));
	vFragColor += mix(.4, 1., v) * vec4(0.8 * t * t * t, 1.4 * t * t, t, 1.0) * textureColor;
	vFragColor += textureColor;
}

	
    