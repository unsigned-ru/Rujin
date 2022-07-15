#include "TronPCH.h"
#include "Rujin.h"
#include "Tron.h"

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

int main()
{
    Tron tron{};
    rujin::Rujin engine{&tron};

    engine.Run();

	return 0;
}
