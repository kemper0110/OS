#include <iostream>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <__msvc_chrono.hpp>
//#include <boost/interprocess/sync/lock>

#include <Semaphore/Semaphore.h>

int main()
{
	using namespace boost::interprocess;

	//struct shm_remove
	//{
	//	shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	//	~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	//} remover;

	managed_shared_memory segment(open_or_create, "MySharedMemory", 1024);
	std::cout << "segment ok\n";

	using ShmemAllocator = allocator<int, managed_shared_memory::segment_manager>;
	using SharedList = list<int, ShmemAllocator>;

	const auto alloc_inst = segment.get_segment_manager();
	SharedList* list = segment.find_or_construct<SharedList>("List")(alloc_inst);;

	//struct mtx_remove
	//{
	//	mtx_remove() { named_mutex::remove("mtx"); }
	//	~mtx_remove() { named_mutex::remove("mtx"); }
	//} remover2;
	//named_mutex mtx{ open_or_create, "mtx" };
	//std::cout << "mtx ok\n";

	Semaphore semaphore(1, 1, L"ListSemaphore");

	for (auto d = .0; d != 1.; d += .1) {	// for(;;)
		Sleep(10'000);
		{
			semaphore.acquire();
			//scoped_lock<named_mutex> lock(mtx);
			list->sort();
			for (const auto v : *list)
				std::cout << v << ' ';
			std::cout << '\n';
			semaphore.release();
		}
	}

	segment.destroy<SharedList>("List");
}
