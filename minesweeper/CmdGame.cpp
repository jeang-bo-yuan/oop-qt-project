/************************//**
 * @file CmdGame.cpp
 * @brief 負責Command Line Game的流程
 ***************************/
#include "common.h"
#include "GameBoard.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

// three modes
static void standby(GameBoard&);
static void playing(GameBoard&);
static int ending(GameBoard&);

/**********************//**
 * @brief startCmdGame
 * @return EXIT_SUCCESS
 * @details 依序呼叫 standby(), playing(), ending()
**************************/
int startCmdGame() {
    GameBoard board;

    while(1) {
        board.unload();

        standby(board);
        playing(board);
        int need_restart = ending(board);
        if (need_restart)
            continue;
        else
            break;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief standby mode
 * @details 可處理的command:
 * - Load BoardFile <file>
 * - Load RandomCount <R> <C> <炸彈數量>
 * - Load RandomRate <R> <C> <炸彈生成機率>
 * - Print ...
 * - StartGame
 */
void standby(GameBoard& board) {
    string input;
    smatch matchResults;

    while (getline(cin, input)){
        bool status = false;

        // Load Boardfile
        if (regex_match(input, matchResults, iregex("Load +Boardfile +([^ ]+)"))){
            status = board.load(matchResults[1]);
        }
        // Load RandomCount <R> <C> <bombs>
        else if (regex_match(input, matchResults, iregex("Load +RandomCount +([0-9]+) +([0-9]+) +([0-9]+)"))) {
            status = board.load((unsigned)stoul(matchResults[1]),
                                (unsigned)stoul(matchResults[2]),
                                (unsigned)stoul(matchResults[3]));
        }
        // Load RandomRate <R> <C> <rate>
        else if (regex_match(input, matchResults, iregex("Load +RandomRate +([0-9]+) +([0-9]+) +([0-9.]+)"))) {
            status = board.load((unsigned)stoul(matchResults[1]),
                                (unsigned)stoul(matchResults[2]),
                                stof(matchResults[3]));
        }
        // Print ...
        else if (regex_match(input, matchResults, iregex("Print +([^ ]+)"))) {
            printCommand(matchResults[1], board, "Standby");
            continue;
        }
        // StartGame
        else if (iequal(input, "StartGame")) {
            if (board.isloaded()) {
                cout << "Success\n";
                return;
            }
        }

        printCommandSuccessOrNot(input, status);
    }

}

/**
 * @brief playing mode
 * @details 可處理的command：
 * - LeftClick <row> <col>
 * - RightClick <row> <col>
 * - Print ...
 */
void playing(GameBoard& board) {
    string input;
    smatch matchResults;

    while (!(bool)board.gameOver() && getline(cin, input)) {
        bool status = false;

        // LeftClick <row> <col>
        if (regex_match(input, matchResults, iregex("LeftClick +([0-9]+) +([0-9]+)"))) {
            status = board.leftClick((unsigned)stoul(matchResults[1]), (unsigned)stoul(matchResults[2]));
        }
        // RightClick <row> <col>
        else if (regex_match(input, matchResults, iregex("RightClick +([0-9]+) +([0-9]+)"))) {
            status = board.rightClick((unsigned)stoul(matchResults[1]), (unsigned)stoul(matchResults[2]));
        }
        // Print ...
        else if (regex_match(input, matchResults, iregex("Print +([^ ]+)"))) {
            printCommand(matchResults[1], board, "Playing");
            continue;
        }

        printCommandSuccessOrNot(input, status);
    }

    if (board.gameOver() == GameBoard::GameOver::win)
        cout << "You win the game\n";
    else
        cout << "You lose the game\n";
}

/**
 * @brief ending mode
 * @return 1 -> need restart;
 *         0 -> no need
 * @details 可處理的command：
 * - Replay
 * - Quit
 * - Print ...
 */
int ending(GameBoard& board) {
    string input;
    smatch matchResults;

    while (getline(cin, input)) {
        bool status = false;

        // Replay
        if (iequal(input, "Replay")) {
            cout << "Success\n";
            return 1;
        }
        // Quit
        else if (iequal(input, "Quit")) {
            cout << "Success\n";
            return 0;
        }
        // Print
        else if (regex_match(input, matchResults, iregex("Print +([^ ]+)"))){
            printCommand(matchResults[1], board, "GameOver");
            continue;
        }

        printCommandSuccessOrNot(input, status);
    }

    return 0;
}

