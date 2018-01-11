// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "vgl.h"

#define PARTSNUM 18
typedef struct{
	int parent;
	int child;
}Parts;
class Gundam{
public :
	Gundam();
	Gundam(char* vs,char* fs);
	void move(mat4 model);
	void walk();
	void idle();
	void getModels(mat4 (&arr)[PARTSNUM]);

private:
	Parts part[PARTSNUM];
	mat4 _position;
	float _alpha;
	float _beta;
	float _gamma;
	float alphas[PARTSNUM];
	float betas[PARTSNUM];
	float gammas[PARTSNUM];
	float Rs[PARTSNUM];

	mat4 Translation[PARTSNUM];
	mat4 Models[PARTSNUM];
	void init();
	void update(int partsNo,float r,float,float,float);
};