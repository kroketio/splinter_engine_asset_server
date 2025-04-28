#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace sourcepp::detail {

bool contains(std::string_view s, char c);

void ltrim(std::string& s);

void rtrim(std::string& s);

void trim(std::string& s);

void ltrim(std::string& s, std::string_view c);

void rtrim(std::string& s, std::string_view c);

void trim(std::string& s, std::string_view c);

std::vector<std::string> split(std::string_view s, char delim);

} // namespace sourcepp::detail
