// Invaders,Invaders, fragment shader by movAX13h, Nov.2015

vec3 color = vec3(0.2, 0.42, 0.68); // blue 1
//vec3 color = vec3(0.1, 0.3, 0.6); // blue 2
//vec3 color = vec3(0.6, 0.1, 0.3); // red
//vec3 color = vec3(0.1, 0.6, 0.3); // green

uniform float iGlobalTime;  
vec2 iResolution = vec2(512., 512.); 
out vec4 vFragColor;

float rand(float x) { return fract(sin(x) * 4358.5453123); }
float rand(vec2 co) { return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5357); }

float invader(vec2 p, float n)  
{  
   p.x = abs(p.x);  
   p.y = floor(p.y - 5.0);  
     
   // 2^x  
   float tmp = exp2(floor(p.x - 3.0 * p.y));  
     
   return step(p.x, 2.0)  
         * step(1.0, floor( mod(n / tmp, 2.0) ));  
}  

float ring(vec2 uv, float rnd)  
{  
   float t = 0.6 * (iGlobalTime + 0.2 * rnd);  
   float i = floor(t / 2.0); // 确保圆心在某时间范围内不变  
     
   // 随机圆心位置  
   vec2 pos =  2.0 * vec2(rand(i * 0.123), rand(i * 2.371)) - 1.0;  
      
   // 动态放大半径：  
   // length(uv - pos)表示圆，t在增大，需要更大的uv才使得 diff 为 0，  
   // t有一定的随机性，可以实现圆环的闪烁效果  
   float diff = length(uv - pos) - mod(t, 2.0);  
   return 1. - smoothstep(0.0, 0.2, abs(diff) );   
}  
void main()  
{  
   vec2 p = gl_FragCoord.xy;  
   vec2 xy = gl_FragCoord.xy / iResolution.xy;  
   vec2 uv = p / iResolution.xy - 0.5;  
   p.y += 120.0*iGlobalTime;   // 下落的速度  
   float r = rand(floor(p/8.0));   // 控制火星文和光环的随机闪烁  
     
   // 用于生成火星文  
   vec2 ip = mod(p, 8.0) - 4.0;  
     
   // 背景中心到四周的颜色渐变  
   float a = -.2 * smoothstep(0.1, 0.8, length(uv));  
     
   // invader * 光环  
   float b = invader(ip, 809999.0*r) * (0.06 + 0.3*ring(uv,r));  
     
   // 火星文雨的闪烁高光  
   float c = color * invader(ip, 809999.0*r) * max(0.0, 0.2*sin(10.0*r*iGlobalTime));  
     
   a += b;  
   a += c;  
   
   vFragColor = vec4( color + vec3(a), 1.0);  
}
     