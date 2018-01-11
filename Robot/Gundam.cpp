#include "Gundam.h"
#define DOR(angle) (angle*3.1415/180);

mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Gundam::init(){
}

Gundam::Gundam(){
	_position = mat4(1.0);
	_alpha = 0;
	_beta = 0;
	_gamma = 0;

	//左手
	part[1].child = 2;part[1].parent = 0;
	part[2].child = 3;part[2].parent = 1;
	part[3].child = -1;part[3].parent = 2;
	part[4].child = -1;part[4].parent = 0;
	//頭
	part[5].child = -1;part[5].parent = 0;
	//右手
	part[6].child = 7;part[6].parent = 0;
	part[7].child = 8;part[7].parent = 6;
	part[8].child = -1;part[8].parent = 7;
	part[9].child = -1;part[9].parent = 0;
	//back
	part[10].child = -1;part[10].parent = 0;
	//下半身
	part[11].child = -1;part[11].parent = 0;
	//左腳
	part[12].child = 13;part[12].parent = 11;
	part[13].child = 14;part[13].parent = 12;
	part[14].child = -1;part[14].parent = 13;
	//右腳
	part[15].child = 16;part[15].parent = 11;
	part[16].child = 17;part[16].parent = 15;
	part[17].child = -1;part[17].parent = 16;

	for(int i = 0;i < PARTSNUM;i++){
		alphas[i] = 0;
		betas[i] = 0;
		gammas[i] = 0;
	}
	Rs[1] = Rs[6] = -3;//上手臂半徑-3
	Rs[2] = Rs[7] = -6;//下手臂半徑-6

}
Gundam::Gundam(char* vs,char *fs){
	
}
void Gundam::getModels(mat4 (&arr)[PARTSNUM]){
	for(int i = 0 ; i < PARTSNUM ; i++)
		arr[i] = Models[i];
}
void Gundam::walk(){
}
void Gundam::idle(){
	mat4 Rotatation;
	mat4 Translation;
	for(int i = 0 ; i < PARTSNUM;i++){
		Models[i] = mat4(1.0f);
	}
	Rotatation = mat4(1.0f);
	Translation = mat4(1.0f); 

	Rotatation = rotate(_alpha,1,0,0)*rotate(_beta,0,1,0)*rotate(_gamma,0,0,1);
	Translation = translate(0,2.9,1)*_position;
	Models[0] = Translation*Rotatation;
	update(1,3,_alpha,_beta,_gamma);//左手
	update(6,3,_alpha,_beta,_gamma);//右手
	update(12,3,_alpha,_beta,_gamma);//左腳
	update(15,3,_alpha,_beta,_gamma);//右腳
	
}
void Gundam::update(int partsNo,float r,float alpha,float beta,float gamma){
	mat4 Rotatation = mat4(1.0);
	//mat4 Translation = mat4(1.0);
	float pitch,yaw,roll;
	
	int pa = part[partsNo].parent;
	pitch = DOR(alpha);
	yaw = DOR(beta);
	roll = DOR(gamma);
	alpha += alphas[partsNo];
	beta += betas[partsNo];
	gamma += gammas[partsNo];
	Rotatation = rotate(alpha,1,0,0)*rotate(beta,0,1,0)*rotate(gamma,0,0,1);
	Translation[partsNo] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[pa];
	
	Models[partsNo] = Translation[partsNo]*Rotatation;

	if(part[partsNo].child != -1)
		update(part[partsNo].child,float r,float alpha,float beta,float gamma){
}
