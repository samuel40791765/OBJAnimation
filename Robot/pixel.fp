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
uniform vec2 resolution = vec2(800., 600.);;

float nrand (vec2 co)
{	
	float a = fract(cos(co.x * 8.3e-3 + co.y) * 4.7e5);
   	float b = fract(sin(co.x * 0.3e-3 + co.y) * 1.0e5);
	return a * .5 + b * .5;
}

float genstars (float starsize, float density, float intensity, vec2 seed)
{
	float rnd = nrand(floor(seed * starsize));
	float stars = pow(rnd,density) * intensity;
	return stars;
}
vec3 Blue = vec3(0,0,1);
void main (void)
{
	vec4 textureColor = texture2D(textureP, UV);
	float r1,g1,b1,r2,g2,b2,r3,g3,b3,r4,g4,b4;
	r1=abs(sin(time*1.0));
	g1=abs(sin(time*1.7));
	b1=abs(sin(time*2.3));//r,g,bの値をsinで変化
	r2=abs(sin(time*2.0));
	g2=abs(sin(time*1.5));
	b2=abs(sin(time*1.7));
	r3=abs(sin(time*3.0));
	g3=abs(sin(time*2.7));
	b3=abs(sin(time*1.5));//r,g,bの値をsinで変化
	r4=abs(sin(time*3.1));
	g4=abs(sin(time*1.3));
	b4=abs(sin(time*1.6));
	vec2 offset = vec2(time * 1.25,0);
	
	vec2 p = 2.0 * (gl_FragCoord.xy / resolution) - 1.0;
	p.x *= resolution.x / resolution.y;
	
	p *= 750.0;
		
	float intensity1 = genstars(0.025, 16.0, 2.5, p + offset * 40.);
	float intensity2 = genstars(0.025, 16.0, 1.5, p + offset * 20.);
	float intensity3 = genstars(0.025, 16.0, 1.0, p + offset * 10.);
	float intensity4 = genstars(0.025, 16.0, 3.0, p + offset * 60.);
	
	vFragColor = vec4(intensity1 *vec3(r1,g1,b1)+intensity2 *vec3(r2,g2,b2)+intensity3 *vec3(r3,g3,b3)+intensity4 *vec3(r4,g4,b4), 1);//(r,g,b)の値で色の変化
	vFragColor += textureColor;
}