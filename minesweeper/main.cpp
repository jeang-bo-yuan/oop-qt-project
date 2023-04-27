/*************************************//**
 * @mainpage Minesweeper
 * 這是踩地雷的文件，程式的進入點在 main.cpp
 * @image html Include_Path.svg "Include關係圖" height=800
*****************************************/
/*************************************//**
 * @file main.cpp
 * @brief 程式的進入點
 * @image html main_process.svg "主要流程"
*****************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <string>

//! \details defined in CmdGame.cpp
extern int startCmdGame();

//! \details defined in GUIGame.cpp
extern int startGUIGame(int argc, char* argv[]);

static void assert_file_open(FILE* fp, const std::string& name = "");

/***************************************//**
 * @brief Process command line arguments and transfer control to \ref startCmdGame or \ref startGUIGame
 * @param argc
 * @param argv
 * @return EXIT_FAILURE or EXIT_SUCCESS
********************************************/
int main(int argc, char *argv[])
{
    // minesweeper.exe CommandInput
    if (argc == 2 && argv[1] == std::string("CommandInput")) {
        std::cout << "Hello\n";
    }
    // minesweeper.exe CommandFile <in> <out>
    else if (argc == 4 && argv[1] == std::string("CommandFile")) {
        assert_file_open(freopen(argv[2], "r", stdin), argv[2]);
        assert_file_open(freopen(argv[3], "w", stdout), argv[3]);
    }
    // minesweeper.exe GUI
    else if (argc == 1 || (argc == 2 && argv[1] == std::string("GUI"))) {
        return startGUIGame(argc, argv);
    }
    else {
        std::cerr << "arg error!!\n"
                     "\tminesweeper.exe CommandInput\n"
                     "\tminesweeper.exe CommandFile <in> <out>\n"
                     "\tminesweeper.exe GUI\n";
        return EXIT_FAILURE;
    }

    return startCmdGame();
}

/**
 * @brief assert fp is open
 * @param fp - the file
 * @param name - the name of file
 * @post print "File <name> cannot be opened" when fp is NULL
 */
static void assert_file_open(FILE* fp, const std::string& name) {
    if (fp == NULL) {
        std::cerr << "File " << name << " cannot be opened\n";
        exit(EXIT_FAILURE);
    }
}
