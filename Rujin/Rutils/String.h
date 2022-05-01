#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#define STRINGSTREAM_STRING(text) static_cast<std::stringstream&&>(std::stringstream{} << text).str()


namespace rutils
{
    std::string Indent(const std::string& src, size_t level, char delimiter = ' ');
    std::string PrefixFill(const std::string& src, const int desiredLength, const char fillChar = '0');
    std::string SuffixFill(const std::string& src, const int desiredLength, const char fillChar = '0');
    std::string ToLower(const std::string& src);
    std::string ShrinkToFit(const std::string& src, const size_t maxLength, const std::string& truncation = "...");
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    std::string LoadFileData(const std::string& path);
    std::vector<std::string> SplitStringByDelimiters(const std::string& string, const std::string& delimiter);


    //template<typename iterator>
    //std::string Join(iterator begin, iterator end, std::string delimiter = ", ")
    //{
    //    std::ostringstream vts;
    //    if (begin == end) 
    //        return vts.str();

    //    // Convert all but the last element to avoid a trailing "," 
    //    std::copy(begin, end - 1, std::ostream_iterator<std::string>(vts, delimiter.c_str()));

    //    // Now add the last element with no delimiter 
    //    vts << *(end - 1);

    //    return vts.str();
    //}
}