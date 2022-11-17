#pragma once
#include <thread>
#include "vector.h"
#include "object.h"

#define MIN_RAND_OBJECTS	10
#define MAX_RAND_OBJECTS	1000

#define OBJ_POS_MIN			-100000
#define OBJ_POS_MAX			 100000

#define OBJ_MASS_MIN		10
#define OBJ_MASS_MAX		100000000

#define OBJ_SIZE_MIN		0
#define OBJ_SIZE_MAX		100

#define OBJ_SSPEAD_MIN		-10000
#define OBJ_SSPEAD_MAX		10000

#define g					9.81


class Nobj {
	std::thread**	threads;
	void create_elements(int _obj_count);
	unsigned int SCREEN_WIDTH = 1920;
	unsigned int SCREEN_HEIGHT = 1080;

public:
	int		obj_count;
	MyObj*  objects_old;
	MyObj*	objects;

	volatile     int	thread_count;
	volatile     bool	thread_is_running;
	volatile     bool	thread_can_progress;
	volatile     bool	thread_all_finnished;
	volatile     bool	thread_all_resetted;
	volatile     double	thread_dt;
	volatile     int	thread_finished_count;
	volatile     int	thread_resetted_count;

	Nobj(unsigned int _SCREEN_WIDTH, unsigned int _SCREEN_HEIGHT);
	Nobj(unsigned int _SCREEN_WIDTH, unsigned int _SCREEN_HEIGHT, int _obj_count);
	~Nobj();
	
	void CreateThreads();
	void CreateThreads(int n);
	void StopThreads();

	void ProgressAll(double dt);
	void ProgressOnThreads(double dt);
	void SyncObjects();
	
	int	 N();

	MyObj* GetObjects();
	MyObj* GetObject(int index);

	void DrawCircle(MyObj* obj);
};

