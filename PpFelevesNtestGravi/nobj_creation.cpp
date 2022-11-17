#include "nobjp.h"

#define RANDOM(min, max) (rand() % (max - min) + min)

void Nobj::create_elements(int _obj_count) {
	obj_count = _obj_count;

	objects_old = new MyObj[obj_count];
	objects = new MyObj[obj_count];
	//std::cout << "Creating NOBJ with " << obj_count << " elements." << std::endl;

	for (int i = 0; i < obj_count; i++) {
		objects[i].pos.x = (double)RANDOM(OBJ_POS_MIN, OBJ_POS_MAX);
		objects[i].pos.y = (double)RANDOM(OBJ_POS_MIN, OBJ_POS_MAX);
		objects[i].pos.z = (double)RANDOM(OBJ_POS_MIN, OBJ_POS_MAX);

		objects[i].m = (double)RANDOM(OBJ_MASS_MIN, OBJ_MASS_MAX);
		objects[i].s = (double)RANDOM(OBJ_SIZE_MIN, OBJ_SIZE_MAX);

		objects[i].v.x = (double)RANDOM(OBJ_SSPEAD_MIN, OBJ_SSPEAD_MAX);
		objects[i].v.y = (double)RANDOM(OBJ_SSPEAD_MIN, OBJ_SSPEAD_MAX);
		objects[i].v.z = (double)RANDOM(OBJ_SSPEAD_MIN, OBJ_SSPEAD_MAX);

		objects[i].allCircleVertices = new float[ALL_CIRCLE_VERTICES];

		objects_old[i] = objects[i];
	}
}

Nobj::Nobj(unsigned int _SCREEN_WIDTH, unsigned int _SCREEN_HEIGHT, int _obj_count) {
	create_elements(_obj_count);
	SCREEN_WIDTH  = _SCREEN_WIDTH;
	SCREEN_HEIGHT = _SCREEN_HEIGHT;
}

Nobj::Nobj(unsigned int _SCREEN_WIDTH, unsigned int _SCREEN_HEIGHT) {
	int random = RANDOM(MIN_RAND_OBJECTS, MAX_RAND_OBJECTS);
	//std::cout << "Creating NOBJ with " << random << " elements." << std::endl;
	create_elements(random);
	SCREEN_WIDTH = _SCREEN_WIDTH;
	SCREEN_HEIGHT = _SCREEN_HEIGHT;
}

Nobj::~Nobj() {
	delete[] objects_old;
	delete[] objects;
	obj_count = 0;
}
