#include "RujinPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "Rujin.h"

int main(int, char*[])
{
	//TODO: Some way to link the engine to the game application that the engine has to run.

	rujin::Rujin engine;
	engine.Run();
    return 0;
}