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
	vec2 p=(gl_FragCoord.xy/iResolution.x)*2.0-vec2(1.0,iResolution.y/iResolution.x);

	p=p*3.0;
   
	float x=p.x;
	float y=p.y;

	float a=
		makePoint(x,y,3.3,2.9,0.3,0.3,time);
	a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);
	a=a+makePoint(x,y,0.8,0.7,0.4,0.5,time);
	a=a+makePoint(x,y,2.3,0.1,0.6,0.3,time);
	a=a+makePoint(x,y,0.8,1.7,0.5,0.4,time);
	a=a+makePoint(x,y,0.3,1.0,0.4,0.4,time);
	a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);
	a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);
	a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);   
   
	float b=
		makePoint(x,y,1.2,1.9,0.3,0.3,time);
	b=b+makePoint(x,y,0.7,2.7,0.4,0.4,time);
	b=b+makePoint(x,y,1.4,0.6,0.4,0.5,time);
	b=b+makePoint(x,y,2.6,0.9,0.6,0.3,time);
	b=b+makePoint(x,y,0.7,1.4,0.5,0.4,time);
	b=b+makePoint(x,y,0.7,1.7,0.4,0.4,time);
	b=b+makePoint(x,y,0.8,0.5,0.4,0.5,time);
	b=b+makePoint(x,y,1.4,0.7,0.6,0.3,time);
	b=b+makePoint(x,y,0.7,1.3,0.5,0.4,time);

	float c=
		makePoint(x,y,3.7,0.3,0.3,0.3,time);
	c=c+makePoint(x,y,1.9,1.3,0.4,0.4,time);
	c=c+makePoint(x,y,0.8,0.9,0.4,0.5,time);
	c=c+makePoint(x,y,1.2,1.7,0.6,0.3,time);
	c=c+makePoint(x,y,0.3,0.6,0.5,0.4,time);
	c=c+makePoint(x,y,0.3,0.3,0.4,0.4,time);
	c=c+makePoint(x,y,1.4,0.8,0.4,0.5,time);
	c=c+makePoint(x,y,0.2,0.6,0.6,0.3,time);
	c=c+makePoint(x,y,1.3,0.5,0.5,0.4,time);
   
	vec3 d=vec3(a,b,c)/32.0;
   
	vFragColor = vec4(d.x,d.y,d.z,1.0);
	vFragColor += textureColor;
}	
    