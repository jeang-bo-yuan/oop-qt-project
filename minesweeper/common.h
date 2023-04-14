/*************************//**
 * @file common.h
 * @brief 包含常用函式、巨集
*****************************/
#ifndef COMMON_H
#define COMMON_H
#include <GameBoard.h>
#include <regex>

//! case insensitive regex
#define iregex(str) (std::regex(str, std::regex::icase))
//! case insensitive compare
#define iequal(str1, str2) (std::regex_match(str1, iregex(str2)))

/**
 * @brief Process `Print ...` command
 * @param info - information to print
 * @param board - minesweeper GameBoard
 * @param state - current game state
 * @post print "<Print *info*> : Failed" of "<Print *info*> : *information*"
 */
void printCommand(const std::string& info, const GameBoard& board, const char* state);

/**
 * @brief Print out if processing *cmd* is successfull or failed
 * @param cmd - The command that is processed
 * @param status - The return *status* of *cmd*. true -> Success; false-> Failed
 * @post print "<*cmd*> : *<Sucess|Failed>*"
 */
void printCommandSuccessOrNot(const std::string& cmd, bool status);

#endif // COMMON_H
