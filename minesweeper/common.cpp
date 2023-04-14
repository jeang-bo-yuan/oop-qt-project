/******************************************//**
 * \file common.cpp
 * \brief implement functions in common.h
**********************************************/
#include "common.h"
#include <iostream>

using namespace std;


void printCommand(const std::string& info, const GameBoard& board, const char* state) {
    cout << "<Print " << info << "> : ";

    if (iequal(info, "GameState")) {
        cout << state << '\n';
        return;
    }
    else if (board.isloaded()) {
        if (iequal(info, "GameBoard")) {
            cout.put('\n');
            for (unsigned row = 0; row < board.rowSize(); ++row) {
                for (unsigned col = 0; col < board.colSize(); ++col) {
                    cout << board.getMask(row, col) << ' ';
                }
                cout.put('\n');
            }
            return;
        }
        else if (iequal(info, "GameAnswer")) {
            cout.put('\n');
            for (unsigned row = 0; row < board.rowSize(); ++row) {
                for (unsigned col = 0; col < board.colSize(); ++col) {
                    cout << board.getAnswer(row, col) << ' ';
                }
                cout.put('\n');
            }
            return;
        }
        else if (iequal(info, "BombCount")) {
            cout << board.getBombCount() << '\n';
            return;
        }
        else if (iequal(info, "FlagCount")) {
            cout << board.getFlagCount() << '\n';
            return;
        }
        else if(iequal(info, "OpenBlankCount")) {
            cout << board.getOpenBlankCount() << '\n';
            return;
        }
        else if(iequal(info, "RemainBlankCount")) {
            cout << board.getRemainBlankCount() << '\n';
            return;
        }
    }

    cout << "Failed\n";
    return;
}

void printCommandSuccessOrNot(const std::string& cmd, bool status) {
    cout << "<" << cmd << "> : " << (status ? "Success" : "Failed") << '\n';
}

