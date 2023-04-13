
#include "GameBoard.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

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
        cerr << "(rows cols Format error) ";
        return false;
    }

    // allocate memory
    unsigned totals = rows * cols;
    char* ansCopy = new char[totals];
    ans = ansCopy; // point to same place
    mask = new char[totals];
    if (ans == NULL || mask == NULL) {
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
        cerr << "(Too few slots) ";
        return false;
    }

    // init ansCopy by setting number and count bomb
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
            ansCopy[row * cols + col] = char('0' + count);
        }
    }

    loaded = true;
    return true;
}


void GameBoard::unload() {
    loaded = false;
    rows = cols = 0;
    delete [] ans;
    delete [] mask;
    bombCount = flagCount = openBlankCount = remainBlankCount = 0;
}
