#include "RujinPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Rujin.h"

int main(int, char*[]) {
	dae::Rujin engine;
	engine.Run();
    return 0;
}