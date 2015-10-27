#include "precompiled.h"
#include "ThreadPool.h"

// these dependencies can be whacked when test objects are taken out for real tasks
#include "WorkerExample.h"
#include "../EDUtilities/InternalOutput.h"

namespace EDThreadPool {
	ThreadPool* ThreadPool::GetInstance() {
		static ThreadPool thread_pool;
		return &thread_pool;
	}

	ThreadPool::~ThreadPool() {
	}

	ThreadPool::ThreadPool() {
		Init();
	}

	void ThreadPool::Init() {
		// TODO - implement your own version
		//InitSolution();

		num_threads = std::thread::hardware_concurrency();
		run.store(true);

		for ( unsigned int i = 0; i < num_threads; i++ ) {
			threads.push_back(new std::thread(&ThreadPool::Execute, this));
		}
	}

	void ThreadPool::Shutdown() {
		ClearWork();

		// TODO - implement your own version
		//ShutdownSolution();

		run.store(false);
		work_condition.notify_all();

		for ( unsigned int i = 0; i < num_threads; i++ ) {
			threads[i]->join();
			delete threads[i];
		}
		
		// if any test objects were created clear them out before we quit
		for ( auto &t : testObjects ) {
			delete t;
		}
		testObjects.clear();
	}

	void ThreadPool::Execute(void) {
		// TODO - implement your own version
		//ExecuteSolution();
		char str[256];
		sprintf_s(str, 256, "Thread ID: %d", std::this_thread::get_id());
		EDMemoryManager::MemoryManager::GetInstance()->CreateHeap(1000000U, str);
		while ( run ) {
			WorkerThread *worker = nullptr;
			GetWork(&worker);
			if ( worker ) {
				worker->DoWork();
				worker->DoneCallback();
			}
		}
	}

	void ThreadPool::PostWork(WorkerThread* _worker_ptr) {
		// TODO - implement your own version
		//PostWorkSolution(_worker_ptr);
		std::unique_lock<std::mutex> locked(queue_mutex);
		workers.push(_worker_ptr);
		work_condition.notify_all();
	}

	void ThreadPool::GetWork(WorkerThread** _worker_ptr) {
		// TODO - implement your own version
		//GetWorkSolution(_worker_ptr);
		std::unique_lock<std::mutex> locked(queue_mutex);
		work_condition.wait(locked, [&]() { return !workers.empty() || !run.load() ; });

		if ( !workers.empty() ) {
			*_worker_ptr = workers.front();
			workers.pop();
			locked.unlock();
		} else {
			*_worker_ptr = nullptr;
			locked.unlock();
		}

		
	}

	void ThreadPool::ClearWork() {
		// TODO - implement your own version
		//ClearWorkSolution();
		for ( size_t i = 0; i < workers.size(); i++ ) {
			queue_mutex.lock();
			workers.pop();
			queue_mutex.unlock();
		}
	}

} // end of EDThreadPool namespace
