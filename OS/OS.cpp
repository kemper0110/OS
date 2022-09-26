#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <bit>
#include <algorithm>
#include <numeric>
#include <vector>
#include <ranges>

//  : public std::allocator<T>
//template<size_t STACKSIZE, typename T>
//class allocator {
//	T data[STACKSIZE];
//	std::vector<bool> bitmap;
//public:
//	using value_type = T;
//	using size_type = size_t;
//	using pointer = T*;
//	using const_pointer = const T*;
//
//	template<typename _Tp1>
//	struct rebind
//	{
//		using other = allocator<STACKSIZE, _Tp1>;
//	};
//
//	allocator() noexcept : bitmap(STACKSIZE) {}
//	~allocator() noexcept {}
//
//	allocator(const allocator& a) noexcept : allocator() {
//		//bitmap = a.bitmap;
//		//std::copy(a.data, a.data + STACKSIZE, data);
//	}
//
//	template<size_t stacksize, class U>
//	allocator(const allocator& a) noexcept : allocator() {
//		//static_assert(sizeof(U) == sizeof(T));
//		//bitmap = a.bitmap;
//		//std::copy(a.data, a.data + STACKSIZE, data);
//	}
//
//	pointer allocate(size_type n) {
//		using iter_t = std::vector<bool>::iterator;
//
//		iter_t begin, end;
//
//		end = begin = bitmap.begin();
//
//		while (end - begin < n && begin != bitmap.end()) {
//			begin = std::find(begin, bitmap.end(), false);
//			end = std::find(begin, bitmap.end(), true);
//		}
//		if (begin == bitmap.end()) 
//			throw "no memory";
//
//		const auto offset = begin - bitmap.begin();
//		return data + offset;
//	}
//	void deallocate(pointer ptr, size_type n) noexcept {
//		//std::destroy_n(ptr, n);
//		const auto offset = ptr - data;
//		std::fill_n(bitmap.begin() + offset, n, false);
//	}
//};

//template<typename T>
//using allocatorT = allocator<1024, T>;

//
//struct Range {
//	int m_begin, m_end;
//	Range(int begin, int end) : m_begin(begin), m_end(end) {}
//
//	struct Sentinel;
//	struct Iterator;
//
//
//	struct Iterator : std::input_iterator_tag {
//		using difference_type = std::ptrdiff_t;
//		using value_type = int;
//		using pointer = int*;
//		using reference = int;
//		using iterator_category = std::input_iterator_tag;
//
//		friend Sentinel;
//		int value, end;
//		Iterator(int begin, int end) : value(begin), end(end) {}
//		Iterator& operator++() {
//			value++;
//			return *this;
//		}
//		int operator*() { return value; }
//		friend bool operator==(const Iterator& it, const Sentinel& s) { return it.value == it.end; }
//	};
//	struct Sentinel {};
//
//	Iterator begin() { return { m_begin, m_end }; }
//	Sentinel end() { return {}; }
//};
//
//
//template <class T>
//class my_allocator
//{
//	char data[50U * sizeof T]{};
//	std::vector<bool> bitmap = std::vector<bool>(50, false);
//
//public:
//	typedef unsigned int size_type;
//	typedef signed int difference_type;
//	typedef T value_type;
//
//	using propagate_on_container_move_assignment = std::true_type;
//
//	template <class U>
//	struct rebind { typedef my_allocator<U> other; };
//
//	my_allocator() = default;
//
//	template <class U>
//	my_allocator(const my_allocator<U>&) {
//
//	}
//	template <class U>
//	my_allocator<T>& operator=(const my_allocator<U>&) {
//		return *this;
//	}
//
//	T* allocate(size_type n) {
//		std::cout << "from " << this << '\n';
//		auto begin = bitmap.begin(), end = bitmap.begin();
//		while (1) {
//			begin = std::find(end, bitmap.end(), false);
//			end = std::find(begin, bitmap.end(), true);
//			const auto size = std::distance(begin, end);
//
//			if (size >= n )
//				break;
//			// || end == bitmap.end()
//			if (begin == bitmap.end())
//				return nullptr;
//		}
//
//		std::fill_n(begin, n, true);
//		const auto offset = std::distance(bitmap.begin(), begin);
//
//		const auto ptr = reinterpret_cast<T*>(data) + offset;
//		std::cout << ptr << '\n';
//		return ptr;
//	}
//	void deallocate(T* ptr, size_t n) {
//		const auto offset = ptr - reinterpret_cast<T*>(data);
//		std::fill_n(bitmap.begin() + offset, n, false);
//	}
//
//};


#define LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>


#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

typedef struct
{
	OVERLAPPED oOverlap;
	HANDLE hPipeInst;
	TCHAR chRequest[BUFSIZE];
	DWORD cbRead;
	TCHAR chReply[BUFSIZE];
	DWORD cbToWrite;
} PIPEINST, * LPPIPEINST;

VOID DisconnectAndClose(LPPIPEINST);
BOOL CreateAndConnectInstance(LPOVERLAPPED);
BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED);
VOID GetAnswerToRequest(LPPIPEINST);

VOID WINAPI CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED);
VOID WINAPI CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED);

HANDLE hPipe;

int main() {

    // Create one event object for the connect operation. 

    const auto hConnectEvent = CreateEvent(
        NULL,    // default security attribute
        TRUE,    // manual reset event 
        TRUE,    // initial state = signaled 
        NULL);   // unnamed event object 

    if (hConnectEvent == NULL)
    {
        printf("CreateEvent failed with %d.\n", GetLastError());
        return 0;
    }

    OVERLAPPED oConnect = {};
    oConnect.hEvent = hConnectEvent;

    // Call a subroutine to create one instance, and wait for 
    // the client to connect. 

    BOOL fPendingIO = CreateAndConnectInstance(&oConnect);

    while (1)
    {
        // Wait for a client to connect, or for a read or write 
        // operation to be completed, which causes a completion 
        // routine to be queued for execution. 

        DWORD dwWait = WaitForSingleObjectEx(
            hConnectEvent,  // event object to wait for 
            INFINITE,       // waits indefinitely 
            TRUE);          // alertable wait enabled 

        switch (dwWait)
        {
            // The wait conditions are satisfied by a completed connect 
            // operation. 
        case 0:
            // If an operation is pending, get the result of the 
            // connect operation. 

            if (fPendingIO)
            {
                DWORD cbRet;
                BOOL fSuccess = GetOverlappedResult(
                    hPipe,     // pipe handle 
                    &oConnect, // OVERLAPPED structure 
                    &cbRet,    // bytes transferred 
                    FALSE);    // does not wait 
                if (!fSuccess)
                {
                    printf("ConnectNamedPipe (%d)\n", GetLastError());
                    return 0;
                }
            }

            // Allocate storage for this instance. 

            const auto lpPipeInst = (LPPIPEINST)GlobalAlloc(
                GPTR, sizeof(PIPEINST));
            if (lpPipeInst == NULL)
            {
                printf("GlobalAlloc failed (%d)\n", GetLastError());
                return 0;
            }

            lpPipeInst->hPipeInst = hPipe;

            // Start the read operation for this client. 
            // Note that this same routine is later used as a 
            // completion routine after a write operation. 

            lpPipeInst->cbToWrite = 0;
            CompletedWriteRoutine(0, 0, (LPOVERLAPPED)lpPipeInst);

            // Create new pipe instance for the next client. 

            fPendingIO = CreateAndConnectInstance(
                &oConnect);
            break;

            // The wait is satisfied by a completed read or write 
            // operation. This allows the system to execute the 
            // completion routine. 

        case WAIT_IO_COMPLETION:
            break;

            // An error occurred in the wait function. 

        default:
        {
            printf("WaitForSingleObjectEx (%d)\n", GetLastError());
            return 0;
        }
        }
    }
    return 0;

}



// CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as a completion routine after writing to 
// the pipe, or when a new client has connected to a pipe instance.
// It starts another read operation. 

VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten,
    LPOVERLAPPED lpOverLap)
{
    LPPIPEINST lpPipeInst;
    BOOL fRead = FALSE;

    // lpOverlap points to storage for this instance. 

    lpPipeInst = (LPPIPEINST)lpOverLap;

    // The write operation has finished, so read the next request (if 
    // there is no error). 

    if ((dwErr == 0) && (cbWritten == lpPipeInst->cbToWrite))
        fRead = ReadFileEx(
            lpPipeInst->hPipeInst,
            lpPipeInst->chRequest,
            BUFSIZE * sizeof(TCHAR),
            (LPOVERLAPPED)lpPipeInst,
            (LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine);

    // Disconnect if an error occurred. 

    if (!fRead)
        DisconnectAndClose(lpPipeInst);
}

// CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED) 
// This routine is called as an I/O completion routine after reading 
// a request from the client. It gets data and writes it to the pipe. 

VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead,
    LPOVERLAPPED lpOverLap)
{
    LPPIPEINST lpPipeInst;
    BOOL fWrite = FALSE;

    // lpOverlap points to storage for this instance. 

    lpPipeInst = (LPPIPEINST)lpOverLap;

    // The read operation has finished, so write a response (if no 
    // error occurred). 

    if ((dwErr == 0) && (cbBytesRead != 0))
    {
        GetAnswerToRequest(lpPipeInst);

        fWrite = WriteFileEx(
            lpPipeInst->hPipeInst,
            lpPipeInst->chReply,
            lpPipeInst->cbToWrite,
            (LPOVERLAPPED)lpPipeInst,
            (LPOVERLAPPED_COMPLETION_ROUTINE)CompletedWriteRoutine);
    }

    // Disconnect if an error occurred. 

    if (!fWrite)
        DisconnectAndClose(lpPipeInst);
}

// DisconnectAndClose(LPPIPEINST) 
// This routine is called when an error occurs or the client closes 
// its handle to the pipe. 

VOID DisconnectAndClose(LPPIPEINST lpPipeInst)
{
    // Disconnect the pipe instance. 

    if (!DisconnectNamedPipe(lpPipeInst->hPipeInst))
    {
        printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
    }

    // Close the handle to the pipe instance. 

    CloseHandle(lpPipeInst->hPipeInst);

    // Release the storage for the pipe instance. 

    if (lpPipeInst != NULL)
        GlobalFree(lpPipeInst);
}

// CreateAndConnectInstance(LPOVERLAPPED) 
// This function creates a pipe instance and connects to the client. 
// It returns TRUE if the connect operation is pending, and FALSE if 
// the connection has been completed. 

BOOL CreateAndConnectInstance(LPOVERLAPPED lpoOverlap)
{
    LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

    hPipe = CreateNamedPipe(
        lpszPipename,             // pipe name 
        PIPE_ACCESS_DUPLEX |      // read/write access 
        FILE_FLAG_OVERLAPPED,     // overlapped mode 
        PIPE_TYPE_MESSAGE |       // message-type pipe 
        PIPE_READMODE_MESSAGE |   // message read mode 
        PIPE_WAIT,                // blocking mode 
        PIPE_UNLIMITED_INSTANCES, // unlimited instances 
        BUFSIZE * sizeof(TCHAR),    // output buffer size 
        BUFSIZE * sizeof(TCHAR),    // input buffer size 
        PIPE_TIMEOUT,             // client time-out 
        NULL);                    // default security attributes
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("CreateNamedPipe failed with %d.\n", GetLastError());
        return 0;
    }

    // Call a subroutine to connect to the new client. 

    return ConnectToNewClient(hPipe, lpoOverlap);
}

BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
    BOOL fConnected, fPendingIO = FALSE;

    // Start an overlapped connection for this pipe instance. 
    fConnected = ConnectNamedPipe(hPipe, lpo);

    // Overlapped ConnectNamedPipe should return zero. 
    if (fConnected)
    {
        printf("ConnectNamedPipe failed with %d.\n", GetLastError());
        return 0;
    }

    switch (GetLastError())
    {
        // The overlapped connection in progress. 
    case ERROR_IO_PENDING:
        fPendingIO = TRUE;
        break;

        // Client is already connected, so signal an event. 

    case ERROR_PIPE_CONNECTED:
        if (SetEvent(lpo->hEvent))
            break;

        // If an error occurs during the connect operation... 
    default:
    {
        printf("ConnectNamedPipe failed with %d.\n", GetLastError());
        return 0;
    }
    }
    return fPendingIO;
}

VOID GetAnswerToRequest(LPPIPEINST pipe)
{
    wprintf(TEXT("[%d] %s\n"), pipe->hPipeInst, pipe->chRequest);
    StringCchCopy(pipe->chReply, BUFSIZE, TEXT("Default answer from server"));
    pipe->cbToWrite = (lstrlen(pipe->chReply) + 1) * sizeof(TCHAR);
}
