#include "TronPCH.h"
#include "Rujin.h"
#include "Tron.h"

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
//
//using namespace rujin;
//
//
//void StartHeapControl()
//{
//#if defined(DEBUG) | defined(_DEBUG)
//	// Notify user if heap is corrupt
//	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
//
//	// Report detected leaks when the program exits
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	// Set a breakpoint on the specified object allocation order number
//	//_CrtSetBreakAlloc(53596);
//#endif
//}
//
//void DumpMemoryLeaks()
//{
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtDumpMemoryLeaks();
//#endif
//}


int main()
{
    Tron tron{};
    Rujin* pEngine = Rujin::Create(&tron);

    pEngine->Run();

	return 0;
}
