#include "main.h"
//#include <GLTools.h>
//#include "targa.cpp"

//vec3 camera = vec3(0,0,0);
vec3 camera = vec3(0, 0, 20);


//vector<unsigned char *> picture;

GLuint loadBMP(const char * imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
							  // Actual RGB data
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	std::cout << dataPos << " " << imageSize << " " << width << " " << height << std::endl;

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0) { imageSize = width*height * 3; } // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0) { dataPos = 54; } // The BMP header is done that way

	data = new unsigned char[imageSize]; // Create a buffer
	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	if (imageSize / width / height == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		std::cout << "4444" << std::endl;
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		std::cout << "3333" << std::endl;
	}
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	int ActionMenu,ModeMenu,ShaderMenu, pShaderMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("walk",1);
	glutAddMenuEntry("jump", 2);
	glutAddMenuEntry("jump higher", 3);
	glutAddMenuEntry("tornado", 4);
	glutAddMenuEntry("uppercut", 5);
	glutAddMenuEntry("headspin", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ShaderMenu = glutCreateMenu(ShaderMenuEvents);//建立右鍵菜單
											  //加入右鍵物件
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Bling", 1);
	glutAddMenuEntry("Toon", 2);
	glutAddMenuEntry("Pixel", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	pShaderMenu = glutCreateMenu(pShaderMenuEvents);
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Gray", 1);
	glutAddMenuEntry("Colorful", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯


	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("shader", ShaderMenu);
	glutAddSubMenu("p_shader", pShaderMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutMouseFunc(Mouse);
	glutTimerFunc (50, idle, 0); 
	glutMainLoop();
	return 0;
}
void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}
void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	num += 0.1;
	globaltime = (glutGet(GLUT_ELAPSED_TIME));
	//printf("%f\n", globaltime);
	if(action == WALK){
		walk(dummy);
		out = dummy + 1;
		//printf("%d	", dummy);
		//87printf("%d	", out);
		if(out > 15) out = 0;
	}
	else if(action == IDLE){
		standby();
		out = 0;
	}

	else if (action == JUMP) {
		jump(dummy);
		out = dummy + 1;
		//printf("%d	", dummy);
		//printf("%d	", out);
		if (out > 36) out = 0;
	}
	else if (action == SUPER_HIGH) {
		jump_superhigh(dummy);
		out = dummy + 1;
		//printf("%d	", dummy);
		//printf("%d	", out);
		if (out > 37) out = 0;
	}
	else if (action == TORNADO) {
		tornado(dummy);
		out = dummy + 1;
		//printf("%d	", dummy);
		//printf("%d	", out);
		if (out > 36) out = 0;
	}
	else if (action == UPPERCUT) {
		uppercut(dummy);
		out = dummy + 1;
		//printf("%d	", dummy);
		//printf("%d	", out);
		if (out > 37) out = 0;
	}
	glutPostRedisplay();
	if (refresh) {
		refresh = false;
		out = 0;
	}
	glutTimerFunc (50, idle, out); 
}

void standby() {
	for (int i = 0; i < PARTSNUM; i++) {
		angles[0][i] = 0.0f;
	}
}

void uppercut(int frame) {
	switch (frame) {
	case 0:
		position -= 0.1;
		angles[0][9] = 2;
		angles[0][12] = 2;
		angles[2][9] = 70;
		angles[2][12] = -70;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 1:
	case 2:
		position -= 0.1;

		angles[0][9] = 4;
		angles[0][12] = 4;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 3:
	case 4:
		position -= 0.1;

		angles[0][9] = 6;
		angles[0][12] = 6;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 5:
	case 6:
		position -= 0.1;


		angles[0][9] += 8;
		angles[0][12] += 8;
		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 7:
	case 8:
		position -= 0.1;

		angles[0][9] = 10;
		angles[0][12] = 10;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 9:
	case 10:
		position -= 0.1;

		angles[0][9] = 12;
		angles[0][12] = 12;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 11:
	case 12:
		position -= 0.1;

		angles[0][9] = 14;
		angles[0][12] = 14;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 13:
	case 14:
		position -= 0.1;

		angles[0][9] = 16;
		angles[0][12] = 16;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 15:
	case 16:
		position -= 0.1;

		angles[0][9] = 18;
		angles[0][12] = 18;
		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 17:
	case 18:
		position -= 0.1;

		angles[0][9] = 20;
		angles[0][12] = 20;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 19:
		angles[0][9] -= 10;
		angles[0][12] -= 20;
		break;
	case 20:
		position += 0.333;

		angles[0][9] -= 10;
		angles[0][12] -= 45;
		angles[1][0] -= 30;

		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 21:
		angles[1][0] -= 30;
		angles[0][9] -= 10;
		angles[0][12] -= 45;
		break;
	case 22:
		position += 0.333;
		angles[1][0] -= 30;
		angles[0][12] -= 45;

		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 23:
		angles[1][0] -= 30;
		break;
	case 24:
		position += 0.334;
		angles[1][0] -= 30;
		angles[0][12] -= 45;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 25:
		position += 0.9;
		angles[1][0] -= 30;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 26:
		position += 1.5;
		angles[1][0] -= 30;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 27:
		position += 1.2;
		angles[1][0] -= 30;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 28:
		position += 1.0;
		angles[1][0] -= 30;

		break;
	case 29:
		position += 0.8;
		angles[1][0] -= 30;
		angles[0][12] += 22.5;
		break;
	case 30:
		position += 0.5;
		angles[1][0] -= 30;
		angles[0][12] += 22.5;
		break;
	case 31:
		position += 0.2;
		angles[1][0] -= 30;
		angles[0][9] += 10;
		angles[0][12] += 22.5;
		break;
	case 32:
		position -= 0.2;


		angles[0][12] += 22.5;

		break;
	case 33:
		position -= 0.5;

		angles[0][12] += 22.5;
		break;
	case 34:
		position -= 0.8;

		angles[0][12] += 22.5;

		break;
	case 35:
		position -= 1.0;
		angles[0][12] += 22.5;
		break;
	case 36:
		position -= 1.2;
		angles[0][12] += 22.5;

		break;
	case 37:
		position -= 1.5;
		printf("%f	", position);
		break;
	case 38:
		break;
	}
}

void jump(int frame) {
	switch (frame) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		position -= 0.1;
		offsets[1][3] += 0.05;
		offsets[1][6] += 0.05;
		break;
	case 19:
	case 20:
		position += 0.333;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 21:
	case 22:
		position += 0.333;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 23:
	case 24:
		position += 0.334;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 25:
	case 26:
		position += 0.3;
		break;
	case 27:
	case 28:
		position += 0.2;
		//offsets[1][3] -= 0.2;
		//offsets[1][6] -= 0.2;
		break;
	case 29:
	case 30:
		position += 0.1;

		break;
	case 31:
	case 32:
		position -= 0.1;

		break;
	case 33:
	case 34:
		position -= 0.2;
		break;
	case 35:
	case 36:
		position -= 0.3;

		break;
	}

}

void tornado(int frame) {
	switch (frame) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		angles[2][9] = 0;
		angles[2][12] = 0;
		angles[1][9] += 40;
		angles[1][12] += 40;
		if (angles[1][9] >= 360)
			angles[1][9] = 0;
		if (angles[1][12] >= 360)
			angles[1][12] = 0;
		break;
	}

}

void jump_superhigh(int frame) {
	switch (frame) {
	case 0:
		position -= 0.1;
		angles[0][9] = 2;
		angles[0][12] = 2;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 1:
	case 2:
		position -= 0.1;

		angles[0][9] = 4;
		angles[0][12] = 4;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 3:
	case 4:
		position -= 0.1;

		angles[0][9] = 6;
		angles[0][12] = 6;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 5:
	case 6:
		position -= 0.1;


		angles[0][9] += 8;
		angles[0][12] += 8;
		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 7:
	case 8:
		position -= 0.1;

		angles[0][9] = 10;
		angles[0][12] = 10;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 9:
	case 10:
		position -= 0.1;

		angles[0][9] = 12;
		angles[0][12] = 12;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 11:
	case 12:
		position -= 0.1;

		angles[0][9] = 14;
		angles[0][12] = 14;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 13:
	case 14:
		position -= 0.1;

		angles[0][9] = 16;
		angles[0][12] = 16;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 15:
	case 16:
		position -= 0.1;

		angles[0][9] = 18;
		angles[0][12] = 18;
		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 17:
	case 18:
		position -= 0.1;

		angles[0][9] = 20;
		angles[0][12] = 20;

		offsets[1][3] += 0.1;
		offsets[1][6] += 0.1;
		break;
	case 19:
		angles[0][9] -= 10;
		angles[0][12] -= 10;
		break;
	case 20:
		position += 0.333;

		angles[0][9] -= 10;
		angles[0][12] -= 10;

		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 21:
		angles[0][9] -= 10;
		angles[0][12] -= 10;
		break;
	case 22:
		position += 0.333;

		angles[0][9] -= 10;
		angles[0][12] -= 10;

		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 23:
		angles[0][9] -= 10;
		angles[0][12] -= 10;
		break;
	case 24:
		position += 0.334;

		angles[0][9] -= 10;
		angles[0][12] -= 10;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 25:
		position += 0.9;

		angles[0][9] -= 10;
		angles[0][12] -= 10;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 26:
		position += 0.7;

		angles[0][9] -= 10;
		angles[0][12] -= 10;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 27:
		position += 0.5;

		angles[0][9] -= 10;
		angles[0][12] -= 10;
		offsets[1][3] -= 0.317;
		offsets[1][6] -= 0.317;
		break;
	case 28:
		position += 0.4;

		angles[0][9] -= 10;
		angles[0][12] -= 10;

		break;
	case 29:
		position += 0.3;

		angles[0][9] -= 10;
		angles[0][12] -= 10;
		break;
	case 30:
		position += 0.2;

		angles[0][9] -= 10;
		angles[0][12] -= 10;

		break;
	case 31:
		position += 0.1;

		angles[0][9] += 15;
		angles[0][12] += 15;
		break;
	case 32:
		position -= 0.1;

		angles[0][9] += 15;
		angles[0][12] += 15;

		break;
	case 33:
		position -= 0.2;

		angles[0][9] += 15;
		angles[0][12] += 15;
		break;
	case 34:
		position -= 0.3;

		angles[0][9] += 15;
		angles[0][12] += 15;

		break;
	case 35:
		position -= 0.4;

		angles[0][9] += 15;
		angles[0][12] += 15;
		break;
	case 36:
		position -= 0.5;

		angles[0][9] += 15;
		angles[0][12] += 15;

		break;
	case 37:
		position -= 0.7;
		printf("%f	", position);
		break;
	}

}

void headspin(int frame) {
	switch (frame) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		position -= 0.1;
		offsets[1][3] += 0.05;
		offsets[1][6] += 0.05;
		break;
	case 19:
	case 20:
		position += 0.333;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 21:
	case 22:
		position += 0.333;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 23:
	case 24:
		position += 0.334;

		offsets[1][3] -= 0.15833;
		offsets[1][6] -= 0.15833;
		break;
	case 25:
	case 26:
		position += 0.3;
		break;
	case 27:
	case 28:
		position += 0.2;
		//offsets[1][3] -= 0.2;
		//offsets[1][6] -= 0.2;
		break;
	case 29:
	case 30:
		position += 0.1;

		break;
	case 31:
	case 32:
		position -= 0.1;

		break;
	case 33:
	case 34:
		position -= 0.2;
		break;
	case 35:
	case 36:
		position -= 0.3;

		break;
	}

}

void walk(int frame) {
	switch (frame) {
	case 0:
		//right arm
		angles[0][9] = -4;
		//left arm
		angles[0][12] = 4;
		//left leg
		angles[0][3] = 3;
		//right leg
		angles[0][6] = -3;

		break;
	case 1:
		angles[0][9] = -8;
		angles[0][12] = 8;

		angles[0][3] = 6;
		angles[0][4] = 12;
		angles[0][6] = -6;
		angles[0][7] = 2;
		break;
	case 2:
		angles[0][9] = -12;
		angles[0][12] = 12;

		angles[0][3] = 9;
		angles[0][4] = 18;
		angles[0][6] = -9;
		angles[0][7] = 3;
		break;
	case 3:
		angles[0][9] = -16;
		angles[0][12] = 16;

		angles[0][3] = 12;
		angles[0][4] = 24;
		angles[0][6] = -12;
		angles[0][7] = 0;
		break;
	case 4:
		angles[0][9] = -12;
		angles[0][12] = 12;

		angles[0][3] = 9;
		angles[0][4] = 18;
		angles[0][6] = -9;
		angles[0][7] = 3;
		break;
	case 5:
		angles[0][9] = -8;
		angles[0][12] = 8;

		angles[0][3] = 6;
		angles[0][4] = 12;
		angles[0][6] = -6;
		angles[0][7] = 6;
		break;
	case 6:
		angles[0][9] = -4;
		angles[0][12] = 4;

		angles[0][3] = 3;
		angles[0][4] = 6;
		angles[0][6] = -3;
		angles[0][7] = 3;
		break;
	case 7:
		angles[0][9] = 0;
		angles[0][12] = 0;

		angles[0][3] = 0;
		angles[0][4] = 0;
		angles[0][6] = 0;
		angles[0][7] = 0;
		break;
	case 8:
		angles[0][9] = 4;
		angles[0][12] = -4;

		angles[0][3] = -3;
		angles[0][4] = 1;
		angles[0][6] = 3;
		angles[0][7] = 6;
		break;
	case 9:
		angles[0][9] = 8;
		angles[0][12] = -8;

		angles[0][3] = -6;
		angles[0][4] = 2;
		angles[0][6] = 6;
		angles[0][7] = 12;
		break;
	case 10:
		angles[0][9] = 12;
		angles[0][12] = -12;

		angles[0][3] = -9;
		angles[0][4] = 3;
		angles[0][6] = 9;
		angles[0][7] = 18;
		break;
	case 11:
		angles[0][9] = 16;
		angles[0][12] = -16;

		angles[0][3] = -12;
		angles[0][4] = 0;
		angles[0][6] = 12;
		angles[0][7] = 24;
		break;
	case 12:
		angles[0][9] = 12;
		angles[0][12] = -12;

		angles[0][3] = -9;
		angles[0][4] = 3;
		angles[0][6] = 9;
		angles[0][7] = 18;
		break;
	case 13:
		angles[0][9] = 8;
		angles[0][12] = -8;

		angles[0][3] = -6;
		angles[0][4] = 6;
		angles[0][6] = 6;
		angles[0][7] = 12;
		break;
	case 14:
		angles[0][9] = 4;
		angles[0][12] = -4;

		angles[0][3] = -3;
		angles[0][4] = 3;
		angles[0][6] = 3;
		angles[0][7] = 6;
		break;
	case 15:
		angles[0][9] = 0;
		angles[0][12] = 0;

		angles[0][3] = 0;
		angles[0][4] = 0;
		angles[0][6] = 0;
		angles[0][7] = 0;
		break;
	}
}


 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;
 
void init(){
	num = 0.0;

	texture = loadBMP("Obj/Android_Robot/android_texture.bmp");
	bg_texture = loadBMP("Obj/Stormtrooper2/M101_hires_STScI-PRC2006-10a.bmp");
	//Obj/Android_Robot/android_texture.bmp
	
	//picture.push_back(vtarga::load_targa("Obj/Stormtrooper2/Stormtrooper_D.tga", picture_format, picture_width, picture_height));

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picture_width, picture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, picture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);

	isFrame = false;
	pNo = 0;
	initaction();
	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "bling.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "test1.fp" },//fragment shader
		{ GL_NONE, NULL }};
	program = LoadShaders(shaders);//讀取shader

	ShaderInfo picture_shaders[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture.fp" },//fragment shader
		{ GL_NONE, NULL } };
	picture_program = LoadShaders(picture_shaders);

	ShaderInfo bling_shaders[] = {
			{ GL_VERTEX_SHADER, "bling.vp" },//vertex shader
			{ GL_FRAGMENT_SHADER,  "bling.fp" },//fragment shader
			{ GL_NONE, NULL } };

	ShaderInfo toon_shaders[] = {
		{ GL_VERTEX_SHADER, "toon.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "toon.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo pixel_shaders[] = {
		{ GL_VERTEX_SHADER, "pixel.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "pixel.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders1[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture1.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders2[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture2.fp" },//fragment shader
		{ GL_NONE, NULL } };
	
	glUseProgram(program);//uniform參數數值前必須先use shader
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");
	ModelID =  glGetUniformLocation(program,"Model");
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	//glUniform1f(glGetUniformLocation(program, "expansion_c"), expansion_co);
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;

	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,25) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,-1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);

	Obj2Buffer();

	//GLuint Texture = loadBMP("Obj/Stormtrooper2/Stormtrooper_N.bmp");
	//UBO
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);  
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);
	glUniformBlockBinding(program, MatricesIdx,0);


	glClearColor(0.0,0.0,0.0,1);//black screen
}

void reloadshader() 
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "bling.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "test1.fp" },//fragment shader
		{ GL_NONE, NULL } };
	//program = LoadShaders(shaders);//讀取shader

	ShaderInfo picture_shaders[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture.fp" },//fragment shader
		{ GL_NONE, NULL } };
	//picture_program = LoadShaders(picture_shaders);

	ShaderInfo bling_shaders[] = {
		{ GL_VERTEX_SHADER, "bling.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "bling.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo toon_shaders[] = {
		{ GL_VERTEX_SHADER, "toon.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "toon.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo pixel_shaders[] = {
		{ GL_VERTEX_SHADER, "pixel.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "pixel.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders1[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture1.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders2[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture2.fp" },//fragment shader
		{ GL_NONE, NULL } };

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	switch (pNo) {
	case 0:

		program = LoadShaders(shaders);//Åª¨úshader

		break;
	case 1:

		program = LoadShaders(bling_shaders);//Åª¨úshader

		break;
	case 2:

		program = LoadShaders(toon_shaders);//Åª¨úshader

		break;
	case 3:

		program = LoadShaders(pixel_shaders);//Åª¨úshader

		break;
	}

	glUseProgram(program);//uniform°Ñ¼Æ¼Æ­È«e¥²¶·¥ýuse shader


	MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
	ModelID = glGetUniformLocation(program, "Model");
	M_KaID = glGetUniformLocation(program, "Material.Ka");
	M_KdID = glGetUniformLocation(program, "Material.Kd");
	M_KsID = glGetUniformLocation(program, "Material.Ks");

	//UBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, UBOsize);
	glUniformBlockBinding(program, MatricesIdx, 0);
}

void reloadpshader()
{
	ShaderInfo picture_shaders[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders1[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture1.fp" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo picture_shaders2[] = {
		{ GL_VERTEX_SHADER, "picture.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER,  "picture2.fp" },//fragment shader
		{ GL_NONE, NULL } };

	switch (oNo) {
	case 0:

		picture_program = LoadShaders(picture_shaders);//Åª¨úshader

		break;
	case 1:

		picture_program = LoadShaders(picture_shaders1);//Åª¨úshader

		break;
	case 2:

		picture_program = LoadShaders(picture_shaders2);//Åª¨úshader

		break;
	}
}

#define DOR(angle) (angle*3.1415/180);
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	//texture = loadBMP("Obj/Stormtrooper2/Stormtrooper_D.bmp");

	int i = rand();

	if (expansion_co > 5.0) {
		delta = -0.5;
	}
	else if (expansion_co <0.0) {
		delta = 0.5;
	}
	expansion_co += delta;

	//glUniform1f(glGetUniformLocation(try_program, "iGlobalTime"), globaltime);
	//glUniform3f(glGetUniformLocation(try_program, "iResolution"), 800, 600, 0);
	//glDrawArrays(GL_POINTS, 0, sizeof(g_vertex_buffer_data) / 8);

	glUseProgram(picture_program);//uniform參數數值前必須先use shader
	glUniform1f(glGetUniformLocation(picture_program, "num"), num);
	glBindTexture(GL_TEXTURE_2D, bg_texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(program);//uniform參數數值前必須先use shader

	//glUniform1f(glGetUniformLocation(program, "iGlobalTime"), i);

	glBindTexture(GL_TEXTURE_2D, texture);
	float eyey = DOR(eyeAngley);
	View       = lookAt(
		               //vec3(0,10,25),// Camera is at (0,0,20), in World Space
					   vec3(eyedistance * sin(eyey), 0, eyedistance * cos(eyey)),
		               vec3(0,0,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	updateModels();
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glUniform1f(glGetUniformLocation(program, "num"), i);


	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,固定點,連續點的偏移量,buffer point)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		for(int j = 0;j <mtls[i].size() ;j++){//
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model

	//globaltime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0);

	/*glUseProgram(try_program);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		4,        // attribute 0
		2,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,        // stride
		(void*)0  // array buffer offset
	);
	glUniform1f(glGetUniformLocation(try_program, "iGlobalTime"), globaltime);
	glUniform3f(glGetUniformLocation(try_program, "iResolution"), 800, 600, 0);
	glDrawArrays(GL_POINTS, 4, sizeof(g_vertex_buffer_data) / 8);
	//glDisableVertexAttribArray(0);*/
	
	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

void Obj2Buffer(){
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	loadMTL("Obj/Android_Robot/body.mtl",Kds,Kas,Kss,Materials,texture);
	//printf("%d\n",texture);
	for(int i = 0;i<Materials.size();i++){
		printf("%d\n", i);
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}
	load2Buffer("Obj/Android_Robot/body.obj", 0);
	load2Buffer("Obj/Android_Robot/head.obj", 1);
	//load2Buffer("Obj/Stormtrooper2/again/head.obj",1);
	load2Buffer("Obj/Android_Robot/leftantenna.obj", 2);
	load2Buffer("Obj/Android_Robot/rightantenna.obj", 4);
	//load2Buffer("Obj/Stormtrooper2/again/hip.obj",2);

	//load2Buffer("Obj/Stormtrooper2/again/left_thigh.obj",3);
	//load2Buffer("Obj/Stormtrooper2/again/left_leg.obj",4);
	load2Buffer("Obj/Android_Robot/leftleg.obj", 3);
	//load2Buffer("Obj/Stormtrooper2/again/left_foot.obj",5);

	//load2Buffer("Obj/Stormtrooper2/again/right_thigh.obj",6);
	//load2Buffer("Obj/Stormtrooper2/again/right_leg.obj",7);
	//load2Buffer("Obj/Stormtrooper2/again/right_foot.obj",8);
	load2Buffer("Obj/Android_Robot/rightleg.obj", 6);

	load2Buffer("Obj/Android_Robot/leftbicep.obj", 12);
	load2Buffer("Obj/Android_Robot/lefthand.obj", 13);
	//load2Buffer("Obj/Stormtrooper2/again/left_lower_arm.obj",10);
	//load2Buffer("Obj/Stormtrooper2/again/left_hand.obj",11);

	load2Buffer("Obj/Android_Robot/rightbicep.obj", 9);
	load2Buffer("Obj/Android_Robot/righthand.obj", 10);
	
	/*load2Buffer("Obj/Stormtrooper2/again/body.obj",0);

	load2Buffer("Obj/Stormtrooper2/again/head.obj",1);

	load2Buffer("Obj/Stormtrooper2/again/hip.obj",2);

	load2Buffer("Obj/Stormtrooper2/again/left_thigh.obj",3);
	load2Buffer("Obj/Stormtrooper2/again/left_leg.obj",4);
	load2Buffer("Obj/Stormtrooper2/again/left_foot.obj",5);

	load2Buffer("Obj/Stormtrooper2/again/right_thigh.obj",6);
	load2Buffer("Obj/Stormtrooper2/again/right_leg.obj",7);
	load2Buffer("Obj/Stormtrooper2/again/right_foot.obj",8);

	load2Buffer("Obj/Stormtrooper2/again/left_upper_arm.obj",9);
	load2Buffer("Obj/Stormtrooper2/again/left_lower_arm.obj",10);
	load2Buffer("Obj/Stormtrooper2/again/left_hand.obj",11);

	load2Buffer("Obj/Stormtrooper2/again/right_upper_arm.obj",12);
	load2Buffer("Obj/Stormtrooper2/again/right_lower_arm.obj",13);
	load2Buffer("Obj/Stormtrooper2/again/right_hand.obj",14);*/

	//load2Buffer("Obj/urightleg.obj",15);	
	//load2Buffer("Obj/drightleg.obj",16);	
	//load2Buffer("Obj/rightfoot.obj",17);
	
	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&uVBO);
	glGenBuffers(1,&nVBO);
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));
		offset[0] += vertices_size[i]*sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		//glBindTexture(GL_TEXTURE_2D, textureFloor);
		//LoadBMPTexture("Stormtrooper_D.bmp", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	mat4 Scale[PARTSNUM];
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f);
		Scale[i] = mat4(1.0f);
	}
	float r, pitch, yaw, roll;
	float alpha, beta = 0, gamma;

	//Body
	Rotatation[0] = rotate(angles[1][0], 0, 1, 0) * rotate(angles[2][0], 0, 0, 1);
	Scale[0] = scale(5, 5, 5);
	Translation[0] = translate(0, position, 0);
	Models[0] = Translation[0] * Rotatation[0] * Scale[0];

	//============================================================
	//ÀY==========================================================
	Translation[1] = translate(0, 1.2, 0);
	Models[1] = Models[0] * Translation[1];
	//============================================================
	//Áv³¡==========================================================
	Translation[2] = translate(0.5,0.7,0);
	Models[2] = Models[1] * Translation[2];
	Translation[4] = translate(-0.5,0.7,0);
	Models[4] = Models[1] * Translation[4];
	//============================================================


	//left leg
	Rotatation[3] = rotate(angles[0][3], 1, 0, 0);
	Translation[3] = translate(0.18, offsets[1][3], 0);
	Models[3] = Models[0] * Translation[3] * Rotatation[3];
	//right leg
	Rotatation[6] = rotate(angles[0][6], 1, 0, 0);
	Translation[6] = translate(-0.18, offsets[1][6], 0);
	Models[6] = Models[0] * Translation[6] * Rotatation[6];


	//=============================================================
	//¥ª¤â
	//right arm
	Rotatation[5] = rotate(angles[1][9], 0, 1, 0);
	Rotatation[9] = rotate(angles[0][9], 1, 0, 0) * rotate(angles[2][9], 0, 0, 1);
	Translation[9] = translate(-1,0.8, 0);
	Models[9] = Models[0] * Rotatation[5] * Translation[9] * Rotatation[9];
	//¥ª¤UÁu
	Translation[10] = translate(-1, 0, 0);
	Models[10] = Models[9] * Translation[10];

	//left arm
	Rotatation[7] = rotate(angles[1][12], 0, 1, 0);
	Rotatation[12] = rotate(angles[0][12], 1, 0, 0) * rotate(angles[2][12], 0, 0, 1);
	Translation[12] = translate(1, 0.8, 0);
	Models[12] = Models[0] * Rotatation[7] * Translation[12] * Rotatation[12];
	//¥k¤UÁu
	Translation[13] = translate(1, 0, 0);
	Models[13] = Models[12] * Translation[13];

}

void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
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
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case '1':
		angle += 5;
		if(angle>=360) angle = 0;
		printf("beta:%f\n",angle);
		break;
	case '2':
		angle -= 5;
		if(angle<=0) angle = 360;
		printf("beta:%f\n",angle);
		break;
	case 'w':
		eyedistance -= 0.2;
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[0][1] -= 5; 
		if(angles[0][1] == -360) angles[0][1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[0][2] -= 5;
		if(angles[0][2] == -360) angles[0][2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option){
	initaction();
	switch(option){
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		action = JUMP;
		break;
	case 3:
		action = SUPER_HIGH;
		break;
	case 4:
		action = TORNADO;
		break;
	case 5:
		action = UPPERCUT;
		break;
	case 6:
		action = HEADSPIN;
		break;
	}
}
void ModeMenuEvents(int option){
	switch(option){
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}
void ShaderMenuEvents(int option){
	pNo = option;
	reloadshader();
	switch (option) {
	case 0:
	
		break;
	case 1:

		break;
	case 2:

		break;
	case 3:

		break;
	}
}

void initaction() {
	
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
	}
	position = -2.0;
	for (int i = 0; i < PARTSNUM; i++)//初始化角度陣列
	{
		angles[0][i] = 0.0;
		angles[1][i] = 0.0;
		angles[2][i] = 0.0;
	}
	angles[2][9] = 50;
	angles[2][12] = -50;

	for (int i = 0; i < PARTSNUM; i++)//初始化角度陣列
	{
		offsets[0][i] = 0.0;
		offsets[1][i] = 0.0;
		offsets[2][i] = 0.0;
	}
	offsets[1][6] = -0.8;
	offsets[1][3] = -0.8;
	refresh = true;
}

void pShaderMenuEvents(int option) {
	oNo = option;
	reloadpshader();
	switch (option) {
	case 0:

		break;
	case 1:

		break;
	case 2:

		break;
	}
}
