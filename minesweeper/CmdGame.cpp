/************************//**
 * @file CmdGame.cpp
 * @brief 負責Command Line的流程
 ***************************/
#include "common.h"
#include "GameBoard.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

static void standby(GameBoard&);
static void playing(GameBoard&);
static int ending(GameBoard&);

static bool printCommand(const std::string& info, const GameBoard& board, const char* state);

/**********************//**
 * @brief startCmdGame
 * @return EXIT_SUCCESS
**************************/
int startCmdGame() {
    GameBoard board;

    //while(1) {
        standby(board);
    /*    playing(board);
        int need_restart = ending(board);
        if (need_restart)
            continue;
        else
            break;
    }*/

    return EXIT_SUCCESS;
}

/**
 * @brief standby mode
 * 可處理的command:
 * - Load BoardFile <file>
 * - Load RandomCount <M> <N> <炸彈數量>
 * - Load RandomRate <M> <N> <炸彈生成機率>
 * - Print ...
 * - StartGame
 */
void standby(GameBoard& board) {
    string input;
    smatch matchResults;

    while (getline(cin, input)){
        cout << "<" << input << "> : ";
        bool status = true;

        // Load Boardfile
        if (regex_match(input, matchResults, iregex("Load +Boardfile +([^ ]+)"))){
            status = board.load(matchResults[1]);
        } else if (regex_match(input, matchResults, iregex("Load +RandomCount +([0-9]+) +([0-9]+) +([0-9]+)"))) {
            cout << "Not implemented yet\n";
        } else if (regex_match(input, matchResults, iregex("Load +RandomRate +([0-9]+) +([0-9]+) +([0-9.]+)"))) {
            cout << "Not implemented yet\n";
        }
        // Print ...
        else if (regex_match(input, matchResults, iregex("Print +([^ ]+)"))) {
            status = printCommand(matchResults[1], board, "Standby");
            if (status)
                continue;
        }
        // StartGame
        else if (iequal(input, "StartGame")) {
            if (board.isloaded()) {
                cout << "Success\n";
                return;
            } else {
                status = false;
            }
        } else {
            status = false;
        }

        cout << (status ? "Success" : "Failed") << '\n';
    }

}

/**
 * @brief playing mode
 * 可處理的command：
 * -
 */
void playing(GameBoard&) {

}

/**
 * @brief ending mode
 * @return 1 -> need restart;
 *         0 -> no need
 * 可處理的command：
 * - Print ...
 * - Replay
 * - Quit
 */
int ending(GameBoard&) {
    string input;
    smatch matchResults;
}

/**
 * @brief printCommand
 * @param info - information to print
 * @return true -> Success;
 *         false -> Failed
 */
bool printCommand(const std::string& info, const GameBoard& board, const char* state) {
    if (iequal(info, "GameState")) {
        cout << state << '\n';
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
        }
        else if (iequal(info, "GameAnswer")) {
            cout.put('\n');
            for (unsigned row = 0; row < board.rowSize(); ++row) {
                for (unsigned col = 0; col < board.colSize(); ++col) {
                    cout << board.getAnswer(row, col) << ' ';
                }
                cout.put('\n');
            }
        }
        else if (iequal(info, "BombCount")) {
            cout << board.getBombCount() << '\n';
        }
        else if (iequal(info, "FlagCount")) {
            cout << board.getFlagCount() << '\n';
        }
        else if(iequal(info, "OpenBlankCount")) {
            cout << board.getOpenBlankCount() << '\n';
        }
        else if(iequal(info, "RemainBlankCount")) {
            cout << board.getRemainBlankCount() << '\n';
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

    return true;
}
