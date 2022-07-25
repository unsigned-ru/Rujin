#include "TronPCH.h"
#include "Rujin.h"
#include "Tron.h"

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

using namespace rujin;

int main()
{
    Tron tron{};
    Rujin* pEngine = Rujin::Create(&tron);

    pEngine->Run();

	return 0;
}
