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
    rujin::Rujin engine{};
    Tron tron{};
    engine.Run(&tron, {});
    return 0;
}
