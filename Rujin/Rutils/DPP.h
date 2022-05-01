#ifdef RUTILS_DPP
#pragma once
#include <string>

#define BIND_THIS_MEMBER_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define CODEBLOCK_INLINE(str) "`" + str + "`"
#define CODEBLOCK_MULTILINE(str) "```" + str + "```"

namespace dpp
{
	class user;
}

namespace rutils
{
	std::string GetUserDisplayName(const dpp::user* user);

}

#endif // RUTILS_DPP


