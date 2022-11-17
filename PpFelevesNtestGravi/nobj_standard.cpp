#include <cstdlib>
#include "nobjp.h"
#include <corecrt_math_defines.h>

int Nobj::N() {
	return obj_count;
}

MyObj* Nobj::GetObjects() {
	return objects;
}
MyObj* Nobj::GetObject(int index) {
	if (index < 0 || index >= obj_count) {
		return NULL;
	}

	return objects + index;
}

void Nobj::ProgressAll(double dt) {
	Vector r, e;
	double stemp;
	int i, j;

	for (i = 0; i < obj_count; i++) {
		objects[i].F.Zeros();
		for (j = 0; j < i; j++) {
			r = objects_old[i].pos - objects_old[j].pos;
			stemp = r.Size();
			e = r * (1 / stemp);
			objects[i].F += e * ((objects_old[i].m * objects_old[j].m * g) / (stemp * stemp));
		}
		for (++j; j < obj_count; j++) {
			r = objects_old[i].pos - objects_old[j].pos;
			stemp = r.Size();
			e = r * (1.0 / stemp);
			objects[i].F += e * ((objects_old[i].m * objects_old[j].m * g) / (stemp * stemp));
		}

		objects[i].v += objects[i].F * (dt / objects[i].m);
		objects[i].pos += objects[i].v * dt;

		DrawCircle(objects + i);
	}

	SyncObjects();
}


float twicePi = 2.0f * M_PI;

void Nobj::DrawCircle(MyObj* obj) {
	float radius = (obj->pos.z / 20000) + obj->s / 5.0;

	for (int i = 0; i < CIRCLE_VERTICES; i++)
	{
		obj->allCircleVertices[i * 3]		= SCREEN_WIDTH  / 2 + obj->pos.x / 200 + (radius * cos(i * twicePi / CIRCLE_SIDES));
		obj->allCircleVertices[(i * 3) + 1] = SCREEN_HEIGHT / 2 + obj->pos.y / 200 + (radius * sin(i * twicePi / CIRCLE_SIDES));
		obj->allCircleVertices[(i * 3) + 2] = 0;
	}
}

void Nobj::SyncObjects() {
	int i;
	for (i = 0; i < obj_count; i++) {
		objects_old[i] = objects[i];
	}
}
