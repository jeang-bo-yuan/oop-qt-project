/*************************************//**
 * @mainpage Minesweeper
 *
*****************************************/
/*************************************//**
 * @file main.cpp
 * @brief 負責處理命令列參數和IO重導向
*****************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <QApplication> // QApplication
#include <QWidget> // QWidget
#include <QHBoxLayout>
#include <QPushButton>

#include <iostream>
#include <stdlib.h>
#include <string>

#include "common.h"

static void assert_file_open(FILE* fp, const std::string& name = "");

/***************************************//**
 * @brief handle command line arguments and call \ref startCmdGame or show window
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
        QApplication a(argc, argv);
        QWidget w;

        QHBoxLayout* layout = new QHBoxLayout;
        w.setLayout(layout);

        layout->addWidget(new QPushButton("Not supported"));
        layout->addWidget(new QPushButton("Close"));

        w.show();
        return a.exec();
    }
    else {
        std::cerr << "arg error!!\n"
                     "\tminesweeper.exe CommandInput\n"
                     "\tminesweeper.exe CommandFile <in> <out>\n"
                     "\tminesweeper.exe GUI\n";
        exit(EXIT_FAILURE);
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
