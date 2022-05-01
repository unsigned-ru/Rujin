#include "stdafx.h"

#ifdef RUTILS_DPP
#include "DPP.h"
#include <sstream>
#include <iomanip>
#include <dpp/dpp.h>

std::string rutils::GetUserDisplayName(const dpp::user* user)
{
	std::stringstream ss{};
	ss << user->username << "#" << std::setw(4) << std::setfill('0') << user->discriminator;
	return ss.str();
}
#endif // RUTILS_CPP
