#include <fstream>
#include <chrono>
#include <iomanip>

#include "MMHeap.h"

namespace EDMemoryManager {

	

	Header * Heap::FindBlock(unsigned int allocSize) {
		//return FindBlockSolution(allocSize);

		Header *cursor = freeListEntry;

		while ( cursor && cursor->size < allocSize ) {
			if ( cursor->next == freeListEntry ) {
				return nullptr;
			}
			cursor = cursor->next;
		}

		return cursor;
	}

	void Heap::Init(unsigned int poolSize, unsigned int _synchronize) {
		synchronize = _synchronize;
		InitializeCriticalSection(&criticalSection);
		MM_HEAP_LOCK;
		// TODO - comment out the solution version and write your own, you can ignore the synchronization code
		//InitSolution(poolSize, _synchronize);

		//Allocated memory for this entire heap
		pool = static_cast<char*>(malloc(poolSize));
		memset(pool, NULL, poolSize);

		firstHeader = reinterpret_cast<Header*>(pool);
		firstHeader->next = firstHeader->prev = firstHeader;
		firstHeader->size = poolSize - (sizeof(Header) + sizeof(Footer));

		lastFooter = reinterpret_cast<Footer*>(&pool[poolSize - sizeof(Footer)]);
		lastFooter->size = firstHeader->size;

		freeListEntry = firstHeader;
		poolSizeTotal = poolSize;

		MM_HEAP_UNLOCK;
	}

	void* Heap::Allocate(unsigned int allocSize) {
		MM_HEAP_LOCK;

		Header* free_head = FindBlock(allocSize);
		Footer* free_foot = nullptr;
		Header* used_head = nullptr;
		Footer* used_foot = nullptr;
		void* data = nullptr;

		if ( free_head != nullptr ) {
			// Split
			if ( free_head->size > allocSize + sizeof(Header) + sizeof(Footer) + 1) {
				free_head->size -= allocSize + sizeof(Header) + sizeof(Footer);

				free_foot = reinterpret_cast<Footer*>(reinterpret_cast<char*>(free_head) + sizeof(Header) + free_head->size);
				free_foot->size = free_head->size;

				used_head = reinterpret_cast<Header*>(reinterpret_cast<char*>(free_foot) + sizeof(Footer));
				used_head->size = allocSize;
				used_head->size |= 1 << 31;
				used_head->next = used_head->prev = nullptr;

				data = reinterpret_cast<void*>(reinterpret_cast<char*>(used_head) + sizeof(Header));	

				used_foot = reinterpret_cast<Footer*>(reinterpret_cast<char*>(used_head) + sizeof(Header) + allocSize);
				used_foot->size = used_head->size;

			// No Split
			} else {
				if ( freeListEntry->next == freeListEntry ) {
					freeListEntry = nullptr;
				} else if ( freeListEntry == free_head ) {
					freeListEntry = freeListEntry->next;
				}

				free_head->prev->next = free_head->next;
				free_head->next->prev = free_head->prev;
				free_head->size |= 1 << 31;

				data = reinterpret_cast<void*>(reinterpret_cast<char*>(free_head) + sizeof(Header));

				//AllocateNoSplitSolution(free_head, &data);
			}

		}
		
		// TODO - comment out the solution version and write your own, you can ignore the synchronization code
		//if ( AllocateSplitSolution(free_head, &data, allocSize) == false ) {
		//	AllocateNoSplitSolution(free_head, &data);
		//}

		MM_HEAP_UNLOCK;
		return data;
	}

	void Heap::DeAllocate(void * data) {
		// MM_HEAP_LOCK;
		bool merged_left = false;
		bool merged_right = false;

		// TODO - comment out the solution version and write your own, you can ignore the synchronization code
		merged_right = DeAllocateRightSolution(data);
		merged_left = DeAllocateLeftSolution(data);

		if ( merged_left != true ) {
			DeAllocateMiddleSolution(data);
		}

		// MM_HEAP_UNLOCK;
	}

	void Heap::CoreDump() {
		// MM_HEAP_LOCK;

		// TODO - comment out the solution version and write your own, you can ignore the synchronization code
		CoreDumpSolution();

		// kick start
		//std::ofstream file("..\\Logs\\coredump.txt", std::ios_base::out | std::ios_base::trunc);
		//if (file.is_open())
		//{
		//	std::time_t time = std::time(nullptr);
		//	std::tm local_time = *std::localtime(&time);
		//	file << "Core Dump on " << std::put_time(&local_time, "%m-%d-%Y %I:%M") << "\n\n";
		//	file << "Mem Address\tData";
		//	file << std::hex;

		//	//some sort of looping mechanism, until it works just print out a few hundred bytes or so
		//	//		print out the memory address - 0x000012ab
		//	//		print out data at the memory address chopped up into bytes		44 12 ab 6e
		//	//		print out the chopped up data interpeted as a character			0=a1
		//	//		0x000012ab		44 12 ab 6e			0=a1

		//	file.close();
		//}

		// MM_HEAP_UNLOCK;
	}
}
