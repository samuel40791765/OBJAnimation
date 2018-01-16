#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#define PARTSNUM 18
#define BODY 0
#define LEFTSHOUDER 1
#define ULEFTARM 2
#define DLEFTARM 3
#define LEFTHAND 4

void updateModels();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);
void pShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( char* obj,int);

void updateObj(int);
void resetObj();
void walk(int);
void standby();
void jump(int);
void jump_superhigh(int);
void tornado(int);
void headspin(int);
void uppercut(int);
void cannon(int);
void initaction();
bool isFrame;


GLuint VAO;
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
GLuint program;
GLuint picture_program;
GLuint try_program;
GLint picture_height, picture_width;
GLenum picture_format;
GLuint texture;
GLuint bg_texture;
GLuint bullet_texture;
GLuint sword_texture;
float expansion_co;
float delta;
float num;
float globaltime;
int out;
bool refresh = false;
bool revealbullet = false;

GLfloat g_vertex_buffer_data[12] = {
	-1,-1,
	-1, 1,
	1, 1,
	-1,-1,
	1, 1,
	1,-1 };
GLuint vertexbuffer;

int pNo;
int oNo;

float angles[3][PARTSNUM];
float offsets[3][PARTSNUM];
float position = -2.0;
float angle = 0.0;
float eyeAngley = 0.0;
float eyedistance = 30.0;
float size = 1;
float movex,movey;
int MatricesIdx;
GLuint ModelID;

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];





std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<GLuint> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection ;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

#define leftHand 0
#define rightHand 1
#define leftFoot 2
#define rightFoot 3
#define WALK 1
#define IDLE 0
#define JUMP 2
#define SUPER_HIGH 3
#define TORNADO 4
#define UPPERCUT 5
#define HEADSPIN 6
#define CANNON 7
int mode;
int action;