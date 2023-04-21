/*********************************//**
 * \file GameBoard.cpp
 * \brief implement GameBoard class
*************************************/
#include "GameBoard.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void GameBoard::basicLoad() {
    // allocate memory
    unsigned totals = rows * cols;
    this->ans = new char[totals];
    this->mask = new char[totals];

    if (ans == NULL || mask == NULL)
        throw GameBoard::LoadFailed("(Not Enough Memory)");

    // init mask
    memset(mask, (char)Mask::closed, totals);
}

//! \pre GameBoard::ans 中只有 GameBoard::Ans::random
void GameBoard::loadRandomly(unsigned clickR, unsigned clickC) {
    if (remainBlankCount > 9) {
        // 讓 (clickR, clickC) 附近不要有地雷
        for (int dR = -1; dR <= 1; ++dR) {
            for (int dC = -1; dC <= 1; ++dC) {
                if (this->onBoard(clickR + dR, clickC + dC)) {
                    this->forceSetAns(clickR + dR, clickC + dC, 'O');
                    --remainBlankCount;
                }
            }
        }
    }

    // 隨機放置地雷與空格
    // 使用「發牌」的原理
    srand((unsigned)time(NULL));
    for (unsigned row = 0; row < rows; ++row) {
        for (unsigned col = 0; col < cols; ++col) {
            if (getAnswer(row, col) == 'O')
                continue;

            unsigned randNum = rand() % (bombCount + remainBlankCount);
            if (randNum < bombCount) {
                forceSetAns(row, col, (char)Ans::mine);
                --bombCount;
            }
            else {
                forceSetAns(row, col, 'O');
                --remainBlankCount;
            }

            if (bombCount + remainBlankCount == 0)
                goto END;
        }
    }

    END:
    putNumberOnAns_CountBombAndBlank();
}

void GameBoard::putNumberOnAns_CountBombAndBlank() {
    bombCount = 0;
    remainBlankCount = 0;
    for (unsigned row = 0; row < rows; ++row) {
        for (unsigned col = 0; col < cols; ++col) {
            if (getAnswer(row, col) == (char)Ans::mine) {
                ++bombCount;
                continue;
            }

            ++remainBlankCount;

            // count bombs around (row, col)
            unsigned count = 0;
            if (onBoard(row - 1, col) && getAnswer(row - 1, col) == (char)Ans::mine) ++count;
            if (onBoard(row - 1, col - 1) && getAnswer(row - 1, col - 1) == (char)Ans::mine) ++count;
            if (onBoard(row, col - 1) && getAnswer(row, col - 1) == (char)Ans::mine) ++count;
            if (onBoard(row + 1, col - 1) && getAnswer(row + 1, col - 1) == (char)Ans::mine) ++count;
            if (onBoard(row + 1, col) && getAnswer(row + 1, col) == (char)Ans::mine) ++count;
            if (onBoard(row + 1, col + 1) && getAnswer(row + 1, col + 1) == (char)Ans::mine) ++count;
            if (onBoard(row, col + 1) && getAnswer(row, col + 1) == (char)Ans::mine) ++count;
            if (onBoard(row - 1, col + 1) && getAnswer(row - 1, col + 1) == (char)Ans::mine) ++count;

            // set
            forceSetAns(row, col, '0' + count);
        }
    }
}

void GameBoard::load(const string& file) {
    this->unload();
    ifstream fin(file);
    if (!fin.is_open())
        throw GameBoard::LoadFailed("(Can't open " + file + ")");

    try {
        // enter rows and cols
        fin >> rows >> cols;
        if (fin.fail())
            throw GameBoard::LoadFailed("(Row or Column Format Error)");

        this->basicLoad();

        // init ans by reading chars
        char input;
        for (unsigned row = 0; row < this->rows; ++row) {
            for (unsigned col = 0; col < this->cols; ++col) {
                fin >> input;
                this->forceSetAns(row, col, input);
            }
        }

        if (fin.fail())
            throw GameBoard::LoadFailed("(Too few slots)");

        // init ans by setting number and count bomb
        this->putNumberOnAns_CountBombAndBlank();

        loaded = true;
    }
    catch(...) {
        this->unload();
        throw;
    }
}

void GameBoard::load(unsigned row, unsigned col, unsigned bomb) {
    this->unload();
    unsigned totals = row * col;
    rows = row;
    cols = col;

    try {
        if (bomb >= totals)
            throw GameBoard::LoadFailed("(Too Many Bombs)");

        if (bomb == 0)
            throw GameBoard::LoadFailed("(Too Few Bombs)");

        this->basicLoad();

        // put random icon
        memset((char*)ans, (char)Ans::random, totals);

        this->bombCount = bomb;
        this->remainBlankCount = totals - this->bombCount;

        loaded = true;
    }
    catch(...) {
        this->unload();
        throw;
    }
}

void GameBoard::load(unsigned row, unsigned col, float rate) {
    this->unload();
    unsigned totals = row * col;
    rows = row;
    cols = col;

    try {
        // check rate
        if (rate <= 0.f || rate >= 1.f)
            throw GameBoard::LoadFailed("(Rate should be between 0 and 1 not inclusively)");

        this->basicLoad();

        // set random icon
        memset((char*)ans, (char)Ans::random, totals);

        // generate random bombCount
        srand((unsigned)time(NULL));
        for (unsigned i = 0; i < totals; ++i) {
            float randFloat = (float)rand() / (float)RAND_MAX;
            if (randFloat < rate)
                ++bombCount;
        }

        if (bombCount == totals) --bombCount;
        remainBlankCount = totals - bombCount;

        loaded = true;
    }
    catch (...) {
        this->unload();
        throw;
    }
}

void GameBoard::unload() {
    loaded = false;
    rows = cols = 0;
    delete [] ans;
    ans = NULL;
    delete [] mask;
    mask = NULL;
    bombCount = flagCount = openBlankCount = remainBlankCount = 0;
    loseGame = false;
}

bool GameBoard::leftClick(unsigned row, unsigned col) {
    if (!onBoard(row, col) // not on board
        || ( // (row, col) is open or flagged
            getMask(row, col) != (char)Mask::closed && (getMask(row, col) != (char)Mask::question)
            )
        )
        return false;

    char ansOfBlock = getAnswer(row, col);

    if (ansOfBlock == (char)Ans::random) {
        this->loadRandomly(row, col);
        ansOfBlock = getAnswer(row, col); // update
    }

    // open and set it as answer
    setMask(row, col, ansOfBlock);

    switch (ansOfBlock) {
    case (char)Ans::mine:
        // 開到地雷
        // print all mine on mask
        for (unsigned row = 0; row < rows; ++row){
            for (unsigned col = 0; col < cols; ++col) {
                if (getAnswer(row, col) == (char)Ans::mine)
                    setMask(row, col, getAnswer(row, col));
            }
        }
        // set as gameover
        loseGame = true;
        break;
    case '0':
        // 擴散
        leftClick(row - 1, col);
        leftClick(row - 1, col - 1);
        leftClick(row, col - 1);
        leftClick(row + 1, col - 1);
        leftClick(row + 1, col);
        leftClick(row + 1, col + 1);
        leftClick(row, col + 1);
        leftClick(row - 1, col + 1);
        [[fallthrough]];
    default:
        // update count
        ++openBlankCount;
        --remainBlankCount;
        break;
    }

    return true;
}

bool GameBoard::rightClick(unsigned row, unsigned col) {
    if (!onBoard(row, col)  // not on board
        || ( // (row, col) is open
            getMask(row, col) != (char)Mask::closed && getMask(row, col) != (char)Mask::flag
            && getMask(row, col) != (char)Mask::question
            )
        )
        return false;

    switch(getMask(row, col)) {
    case (char)Mask::closed:
        setMask(row, col, (char)Mask::flag);
        ++flagCount;
        break;
    case (char)Mask::flag:
        setMask(row, col, (char)Mask::question);
        --flagCount;
        break;
    case (char)Mask::question:
        setMask(row, col, (char)Mask::closed);
        break;
    }

    return true;
}


GameBoard::GameOver GameBoard::gameOver() const {
    if (!isloaded())
        return GameOver::nope;

    if (loseGame)
        return GameOver::lose;
    else if (remainBlankCount == 0)
        return GameOver::win;
    else
        return GameOver::nope;
}
