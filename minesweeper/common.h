/*************************//**
 * @file common.h
 * @brief 包含常用函式、巨集
*****************************/
#ifndef COMMON_H
#define COMMON_H

#include <regex>
//! case insensitive regex
#define iregex(str) (std::regex(str, std::regex::icase))
//! case insensitive compare
#define iequal(str1, str2) (std::regex_match(str1, iregex(str2)))

int start_cmd_game();

#endif // COMMON_H
