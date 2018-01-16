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

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){
	float xx=x+cos(t*fx)*sx;
	float yy=y+sin(t*fy)*sy;
	return 0.10/sqrt(xx*xx+yy*yy);
}

void main( void ) {
	vec4 textureColor = texture2D(textureP, UV);
	vec2 p = 2.0*( gl_FragCoord.xy / iResolution.xy )-1.0;
	p.x *= iResolution.x/iResolution.y;
	vec3 col = vec3(0);
	
	
	for (int i = 0; i < 40; i++) {
		float a = time*1+float(i)*0.1; 
		vec2 c = vec2(sin(a*0.2)*cos(0.1*time),cos(a*0.5)*sin(time*0.01)); 
		float d = 0.2/(0.000+50.0*abs(length(p.xy)-1.0/(1.0-mod(time,4.0)+0.1*float(i)))); 
		col += vec3(0.5,0.3,0)*d; 
		
	}
	vFragColor = vec4(col, 0.2);
	vFragColor += textureColor;
}	
    