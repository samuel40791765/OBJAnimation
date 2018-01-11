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
	//vec4 sum = vec4(0.0);
	vec4 temp_color = vec4(0);
	vec2 tc = fragCoord.xy / iResolution.x;

	float dx = 15.*(1./512.);
    float dy = 10.*(1./512.);
    vec2 XY = vec2(dx*(tc.x/dx), dy*floor(tc.y/dy));
    temp_color = texture(ourTexture, tc).bgra;

    //float blur = 5.0;

    float r = floor(temp_color.r * 3.0) / 3.0;
    float g = floor(temp_color.g * 3.0) / 3.0;
    float b = floor(temp_color.b * 3.0) / 3.0;
    float gray = r * 0.2126 + g * 0.7152 + b * 0.0722;
    /*sum += texture(ourTexture, vec2(tc.x - 4.0*blur, tc.y - 4.0*blur)) * 0.0162162162;
    sum += texture(ourTexture, vec2(tc.x - 3.0*blur, tc.y - 3.0*blur)) * 0.0540540541;
    sum += texture(ourTexture, vec2(tc.x - 2.0*blur, tc.y - 2.0*blur)) * 0.1216216216;
    sum += texture(ourTexture, vec2(tc.x - 1.0*blur, tc.y - 1.0*blur)) * 0.1945945946;

    sum += texture(ourTexture, vec2(tc.x, tc.y)) * 0.2270270270;

    sum += texture(ourTexture, vec2(tc.x + 1.0*blur, tc.y + 1.0*blur)) * 0.1945945946;
    sum += texture(ourTexture, vec2(tc.x + 2.0*blur, tc.y + 2.0*blur)) * 0.1216216216;
    sum += texture(ourTexture, vec2(tc.x + 3.0*blur, tc.y + 3.0*blur)) * 0.0540540541;
    sum += texture(ourTexture, vec2(tc.x + 4.0*blur, tc.y + 4.0*blur)) * 0.0162162162;*/

      
   	//color = texture(ourTexture, tc).bgra;
   	//color.r = gray;
    //color.g = gray;
    //color.b = gray;
    //color = sum;
    //float gray = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    //color = vec4(r*abs(sin(num)),g*abs(sin(num)*cos(num)),b*abs(cos(num)),1.0);
    color = vec4(tc.x*temp_color.r, tc.y*temp_color.g,(0.5+0.5*sin(num))*temp_color.b,1.0);
}