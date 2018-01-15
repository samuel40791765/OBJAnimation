#version 430 core
#define fragCoord gl_FragCoord  
//#define fragColor gl_FragColor
in vec2 uv;

out vec4 color;

uniform sampler2D ourTexture;
uniform float num;
const vec2 iResolution = vec2(800., 600.); 

void main()
{
	vec4 temp_color = vec4(0);
	vec2 tc = fragCoord.xy / iResolution.x;
	float dx = 15.*(1./512.);
    float dy = 10.*(1./512.);
    vec2 XY = vec2(dx*(tc.x/dx), dy*floor(tc.y/dy));
    temp_color = texture(ourTexture, tc).bgra;
	float gray = 0.299 * temp_color.r + 0.587 * temp_color.g + 0.114 * temp_color.b;
	color = vec4(vec3(gray),1.0);

}