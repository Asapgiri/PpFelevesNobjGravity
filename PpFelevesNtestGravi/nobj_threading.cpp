//#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include "nobjp.h"


std::mutex mtx_tfc;
std::mutex mtx_trs;
std::mutex mtx_print;



void RunOnThread(Nobj* p, MyObj* so, int obj_index, int objects_per_thread) {
	Vector r, e;
	double stemp;
	int k, i;

#define print_log(std)/*								\
	mtx_print.lock();								\
	printf(" [ thso %2d ]: "std"\n", obj_index);	\
	mtx_print.unlock()*/

	while (p->thread_is_running) {
		print_log("Starts waiting for p->thread_can_progress");
		while (!p->thread_can_progress) {
			if (!p->thread_is_running) {
				return;
			}
		}
		print_log("Starts progress");

		for (k = 0; k < objects_per_thread; k++) {
			so[k].F.Zeros();

			for (i = 0; i < k; i++) {
				r = so[k].pos - p->objects[i].pos;
				stemp = r.Size();
				e = r * (1 / stemp);
				so[k].F += e * ((so[k].m * p->objects[i].m * g) / (stemp * stemp));
			}
			for (++i; i < objects_per_thread; i++) {
				r = so[k].pos - p->objects[i].pos;
				stemp = r.Size();
				e = r * (1 / stemp);
				so[k].F += e * ((so[k].m * p->objects[i].m * g) / (stemp * stemp));
			}

			so[k].v += so[k].F * (p->thread_dt / so[k].m);
			so[k].pos += so[k].v * p->thread_dt;

			p->DrawCircle(so + k);
		}

		print_log("Finnished progress");
		mtx_tfc.lock();
		p->thread_finished_count++;
		if (p->thread_finished_count == p->thread_count) {
			p->thread_can_progress  = false;
			p->thread_all_finnished = true;
			print_log("Was last thread to finnish progress");
		}
		mtx_tfc.unlock();

		while (!p->thread_all_finnished) {}

		print_log("Resetting thread...");
		mtx_trs.lock();
		p->thread_resetted_count++;
		if (p->thread_resetted_count == p->thread_count) {
			p->thread_all_resetted = true;
			print_log("Was last thread to reset");
		}
		mtx_trs.unlock();
	}
}

void Nobj::ProgressOnThreads(double dt) {
#define print_mlog(std)/*				\
	mtx_print.lock();				\
	printf(" [ main ]: "std"\n");	\
	mtx_print.unlock()*/

	print_mlog("Waiting for threads (but why???)");
	while (!thread_all_finnished) {}

	print_mlog("Set dt on threads");
	thread_dt			  = dt;
	thread_finished_count = 0;
	thread_resetted_count = 0;
	thread_all_resetted	  = false;
	thread_all_finnished  = false;
	thread_can_progress   = true;

	print_mlog("Waiting for threads to finnish");
	while (!thread_all_resetted) {}
	print_mlog("All threads finnished");
}

void Nobj::CreateThreads() {
	auto processor_count = std::thread::hardware_concurrency();

	if (processor_count < 1) {
		processor_count = 2;
	}

	this->CreateThreads(processor_count);
}

void Nobj::CreateThreads(int n) {
	thread_count		  = n;
	thread_is_running	  = true;
	thread_can_progress	  = false;
	thread_all_finnished  = true;
	thread_finished_count = 0;
	thread_resetted_count = 0;

	threads = (std::thread**)malloc(n * sizeof(std::thread*));
	int objects_per_thread = 0;
	int start_object = 0;

	int obj_temp_div;
	int obj_temp_mod;

	if (obj_count < thread_count) {
		thread_count = obj_count;
	}
	if (obj_count >= thread_count) {
		obj_temp_div = obj_count / thread_count;
		obj_temp_mod = obj_count % thread_count;

		for (int i = 0; i < thread_count; i++) {
			start_object += objects_per_thread;
			objects_per_thread = obj_temp_div;
			if (i < obj_temp_mod) objects_per_thread++;

			threads[i] = new std::thread(&RunOnThread, this, &objects[start_object], start_object, objects_per_thread);
		}
	}
}

void Nobj::StopThreads() {
	while (!thread_all_finnished) {}

	thread_is_running = false;

	for (int i = 0; i < thread_count; i++) {
		threads[i]->join();
		delete threads[i];
	}

	thread_count = 0;
	free(threads);
}

