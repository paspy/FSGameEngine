#ifndef _MY_WORKER_H_
#define _MY_WORKER_H_


#include "../EDUtilities/InternalOutput.h"
#include "WorkerThread.h"
#include <iomanip>

namespace EDThreadPool {
	class MyWorker : public WorkerThread {
	public:
		int precision;
		double pi;
		double p16;
	public:
		MyWorker(int _precision) {
			precision = _precision;
			pi = 0;
			p16 = 1;
		}

		void DoneCallback() {
			EDUtilities::InternalOutput::GetReference() << "Thread Pool found Pi:" << pi << "\nwith precision of " << precision <<  "\n";
		}

		void DoWork() {
			for ( int k = 0; k <= precision; k++ ) {
				pi += 1.0 / p16 * (4.0 / (8 * k + 1) - 2.0 / (8 * k + 4) - 1.0 / (8 * k + 5) - 1.0 / (8 * k + 6));
				p16 *= 16;
			}
		}
	};
}

#endif
