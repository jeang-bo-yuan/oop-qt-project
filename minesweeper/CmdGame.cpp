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
 * @details 依序呼叫 standby(), playing(), ending()
**************************/
int startCmdGame() {
    GameBoard board;

    while(1) {
        board.unload();

        standby(board);
        //playing(board);
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
        bool status = false;

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
            }
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
 * @details 可處理的command：
 * - Print ...
 * - Replay
 * - Quit
 */
int ending(GameBoard& board) {
    string input;
    smatch matchResults;

    while (getline(cin, input)) {
        cout << "<" << input << "> : ";
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
            status = printCommand(matchResults[1], board, "GameOver");
            if (status)
                continue;
        }

        cout << (status ? "Success" : "Failed") << '\n';
    }

    return 0;
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
