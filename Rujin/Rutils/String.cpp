#include "String.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>


std::string rutils::Indent(const std::string& src, size_t level, char delimiter)
{
	return std::string(level * 4, delimiter) + src;
}

std::string rutils::PrefixFill(const std::string& src, const int desiredLength, const char fillChar)
{
	return std::string(desiredLength - src.length(), fillChar) + src;
}

std::string rutils::SuffixFill(const std::string& src, const int desiredLength, const char fillChar)
{
	return src + std::string(desiredLength - src.length(), fillChar);
}

std::string rutils::ToLower(const std::string& src)
{
	std::string ret{};
	ret.reserve(src.length());

	std::transform(src.begin(), src.end(), ret.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return ret;
}

std::string rutils::ShrinkToFit(const std::string& src, const size_t maxLength, const std::string& truncation)
{
	if (src.length()  <= maxLength) 
		return src;

	return src.substr(0, maxLength - truncation.length()) + truncation;
}

std::string rutils::ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find_first_of(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, 1, to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

std::string rutils::LoadFileData(const std::string& path)
{
	//check if the file exists
	if (!std::filesystem::exists(path))
		throw std::exception("File not found.");

	std::ifstream fin(path, std::ios::in | std::ios::binary);
	std::ostringstream oss;
	oss << fin.rdbuf();

	return oss.str();
}

std::vector<std::string> rutils::SplitStringByDelimiters(const std::string& string, const std::string& delimiters)
{
	std::vector<std::string> arguments{};
	size_t startPos{ 0 };
	size_t endPos{ 0 };

	while (true)
	{
		//startPos = find first non whitespace character in line with offset of old end position
		if ((startPos = string.find_first_not_of(delimiters, endPos)) == std::string::npos)
			//we couldn't get any start position anymore.
			break;

		//endPos = find first whitespace character in line after startPos.
		if ((endPos = string.find_first_of(delimiters, startPos)) == std::string::npos)
			//no whitespace found after start, the end of the argument will be the end of the line.
			endPos = string.size();

		//create argument
		std::string argument{ string.substr(startPos, endPos - startPos) };
		arguments.push_back(argument);
	}

	return arguments;
}

bool rutils::StartsWith(const std::string& s, const std::string& startsWith)
{
	return s.rfind(startsWith, 0) != std::string::npos;
}
