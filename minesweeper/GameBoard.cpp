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

bool GameBoard::load(const string& file) {
    this->unload();

    // open file
    ifstream fin(file);
    if (!fin.is_open()) {
        cerr << "(Can't open " << file << ") ";
        return false;
    }

    // enter rows and cols
    fin >> rows >> cols;
    if (fin.fail()) {
        this->unload();
        cerr << "(rows cols Format error) ";
        return false;
    }

    // allocate memory
    unsigned totals = rows * cols;
    char* ansCopy = new char[totals];
    ans = ansCopy; // point to same place
    mask = new char[totals];
    if (ans == NULL || mask == NULL) {
        this->unload();
        cerr << "(not enough memory) ";
        return false;
    }

    // init mask
    memset(mask, (char)Mask::closed, totals);

    // init ansCopy by reading (totals) chars
    for (unsigned idx = 0; fin.good() && idx < totals; ++idx) {
        fin >> ansCopy[idx];
    }
    if (fin.fail()) {
        this->unload();
        cerr << "(Too few slots) ";
        return false;
    }

    // init ansCopy by setting number and count bomb
    this->putNumberOnAns_CountBombAndBlank();

    loaded = true;
    return true;
}

bool GameBoard::load(unsigned row, unsigned col, unsigned bomb) {
    this->unload();
    unsigned totals = row * col;
    rows = row;
    cols = col;

    if (bomb >= totals) {
        this->unload();
        cerr << "(too many bombs) ";
        return false;
    }

    if (bomb == 0) {
        this->unload();
        cerr << "(too few bombs) ";
        return false;
    }

    // allocate memory
    char* ansCopy = new char[totals];
    this->ans = ansCopy;
    this->mask = new char[totals];
    if (ansCopy == NULL || mask == NULL) {
        this->unload();
        cerr << "(not enough memory) ";
        return false;
    }

    // init mask
    memset(mask, (char)Mask::closed, totals);

    // init ans
    memset(ansCopy, (char)Ans::mine, bomb);
    memset(ansCopy + bomb, 'O', totals - bomb);
    srand((unsigned)time(NULL));
    for (unsigned idx = 0; idx < totals; ++idx) { // shuffle
        unsigned idx2 = rand() % totals;
        swap(ansCopy[idx], ansCopy[idx2]);
    }

    // init ansCopy by setting number and count bomb
    this->putNumberOnAns_CountBombAndBlank();

    loaded = true;
    return true;
}

bool GameBoard::load(unsigned row, unsigned col, float rate) {
    this->unload();
    unsigned totals = row * col;
    rows = row;
    cols = col;

    // allocate memory
    char* ansCopy = new char[totals];
    this->ans = ansCopy;
    this->mask = new char[totals];
    if (ansCopy == NULL || mask == NULL) {
        this->unload();
        cerr << "(not enough memory) ";
        return false;
    }

    // init mask
    memset(mask, (char)Mask::closed, totals);

    // init ans
    srand((unsigned)time(NULL));
    for (unsigned idx = 0; idx < totals; ++idx) {
        float randomFloat = (float)rand() / (float)RAND_MAX;
        ansCopy[idx] = (randomFloat < rate ? 'X' : 'O');
    }

    // init ansCopy by setting number and count bomb
    this->putNumberOnAns_CountBombAndBlank();

    loaded = true;
    return true;
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
            getMask(row, col) != (char)Mask::closed && (getMask(row, col) != (char)Mask::quetion)
            )
        )
        return false;

    // open and set it as answer
    setMask(row, col, getAnswer(row, col));

    // update count
    --remainBlankCount;
    ++openBlankCount;

    switch (getAnswer(row, col)) {
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
        break;
    }

    return true;
}

bool GameBoard::rightClick(unsigned row, unsigned col) {
    if (!onBoard(row, col)  // not on board
        || ( // (row, col) is open
            getMask(row, col) != (char)Mask::closed && getMask(row, col) != (char)Mask::flag
            && getMask(row, col) != (char)Mask::quetion
            )
        )
        return false;

    switch(getMask(row, col)) {
    case (char)Mask::closed:
        setMask(row, col, (char)Mask::flag);
        ++flagCount;
        break;
    case (char)Mask::flag:
        setMask(row, col, (char)Mask::quetion);
        --flagCount;
        break;
    case (char)Mask::quetion:
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
