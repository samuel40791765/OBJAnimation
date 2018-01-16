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

void main( void ) {
	vec4 textureColor = texture2D(textureP, UV);
	vec2 v_texCoord = gl_FragCoord.xy / iResolution;

    vec2 p =  v_texCoord * 8.0 - vec2(20.0);
    vec2 i = p;
    float c = 1.0;
    float inten = .05;

    for (int n = 0; n < MAX_ITER; n++)
    {
        float t = time * (2.0 - (3.0 / float(n+1))) * 0.1;

        i = p + vec2(cos(t - i.x) + sin(t + i.y),
        sin(t - i.y) + cos(t + i.x));
		    
        c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),
        p.y / (cos(i.y+t)/inten)));
    }

    c /= float(MAX_ITER);
    c = 1.5 - sqrt(c);

    vec4 texColor = vec4(0.50, 0.15, 0.02, 1.);

    texColor.rgb *= (1.0/ (1.0 - (c + 0.05)));

    vFragColor = texColor;
	vFragColor.r *= textureColor.r;
	vFragColor.g *= textureColor.g;
	vFragColor.b *= textureColor.b;
}	
    