#include <iostream>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <chrono>
#include <boost/date_time/posix_time/ptime.hpp>

#include <Semaphore/Semaphore.h>

int main()
{
	using namespace boost::interprocess;

	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;
	
	managed_shared_memory segment(create_only, "MySharedMemory", 1024);
	std::cout << "segment ok\n";

	using ShmemAllocator = allocator<int, managed_shared_memory::segment_manager>;
	using SharedList = list<int, ShmemAllocator>;

	const auto alloc_inst = segment.get_segment_manager();
	const auto collection = { 1, 2, 3, 4, 5, 6 };
	SharedList* list = segment.find_or_construct<SharedList>("List")(collection.begin(), collection.end(), alloc_inst);;



	//struct mtx_remove
	//{
	//	mtx_remove() { named_mutex::remove("mtx"); }
	//	~mtx_remove() { named_mutex::remove("mtx"); }
	//} remover2;
	//named_mutex mtx{ open_or_create, "mtx" };
	//std::cout << "mtx ok\n";

	Semaphore semaphore(1, 1, L"ListSemaphore");

	for (auto d = .0; d != 1.; d += .1) {	// for(;;)
		Sleep(1'000);
		{
			//boost::posix_time::ptime p = boost::posix_time::milliseconds(1000);
			semaphore.acquire();
			if (rand() % 100 < 50)
				list->push_back(rand() % 50);
			else
				if (not list->empty())
					list->pop_front();
			for (const auto v : *list)
				std::cout << v << ' ';
			std::cout << '\n';
			semaphore.release();
		}
	}
	//std::invoke(
//	rand() & 1 ? &SharedList::pop_back : &SharedList::pop_front,
//	list);
	segment.destroy<SharedList>("List");
}
