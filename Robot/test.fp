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
uniform int random;
const float PI = 3.1415926535;
out vec4 vFragColor;

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular

vec2 Distort(vec2 p)
{
    float theta  = atan(p.y, p.x);
    float radius = length(p);
    radius = pow(radius, 2);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

void main(void)
{ 
    float dx = 15.*(1./512.);
    float dy = 10.*(1./512.);
    //int i = rand();
    vec2 XY = vec2(dx*(UV.x/dx), dy*floor(UV.y/dy));

	vec4 textureColor = texture2D(textureP, UV);
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1) * textureColor;

    // Add in ambient light
    vFragColor += ambientColor * textureColor;


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								normalize(vVaryingNormal)));//反射角
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*vec4(Material.Ka,1)*spec  * textureColor;
    }
    //float gray = vFragColor.r * 0.2126 + vFragColor.g * 0.7152 + vFragColor.b * 0.0722;
   vFragColor.r *= abs(sin(random));
    vFragColor.g *= abs(cos(random));
    vFragColor.b *= abs(sin(random*2));
   // vFragColor.r = (floor(int(gray * 4.0)) / 4.0);
   // vFragColor.g = (floor(int(gray * 4.0)) / 4.0);
   // vFragColor.b = (floor(int(gray * 4.0)) / 4.0);
}
	
    