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
const float Eta = 0.15; // Water

uniform MaterialInfo Material;
uniform sampler2D textureP;
out vec4 vFragColor;

//lighting color
vec4    ambientColor = vec4(0.24725, 0.1995, 0.0745, 1);
vec4    diffuseColor = vec4(0.75164, 0.60648, 0.22648, 1);   
vec4    specularColor = vec4(0.628281, 0.555802, 0.366065, 1);
//0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228
//0.24725, 0.1995, 0.0745, 1 0.75164, 0.60648, 0.22648, 1 0.628281, 0.555802, 0.366065, 1

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular

void main(void)
{ 
	vec4 textureColor = texture2D(textureP, UV);
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1);

    // Add in ambient light
    vFragColor += ambientColor * textureColor;


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								normalize(vVaryingNormal)));//反射角
	//vec3 vRefraction = normalize(refract(-normalize(vVaryingLightDir), normalize(vVaryingNormal), Eta));//反射角
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*vec4(Material.Ka,1)*spec  * textureColor;
    }
    //vFragColor.r = (floor(int(vFragColor.r * 4.0)) / 4.0);
    //vFragColor.g = (floor(int(vFragColor.g * 4.0)) / 4.0);
    //vFragColor.b = (floor(int(vFragColor.b * 4.0)) / 4.0);

	//vec3 refraction = refract(worldIncident, worldNormal, Eta);
	//vec3 reflection = reflect(worldIncident, worldNormal);
	
	//vec4 refractionColor = texture(textureP, vReflection);
	//vec4 reflectionColor = texture(textureP, vRefraction);
	
	//float fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(normalize(vVaryingLightDir), normalize(vVaryingNormal))), 5.0);
				
	//vFragColor = mix(refractionColor, reflectionColor, fresnel);
}
	
    