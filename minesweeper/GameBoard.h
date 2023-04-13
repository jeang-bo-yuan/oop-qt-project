#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <stddef.h>

class GameBoard
{
private:
    bool loaded;
    unsigned rows;
    unsigned cols;
    const char* ans;
    char* mask;

    unsigned bombCount;
    unsigned flagCount;
    unsigned openBlankCount;
    unsigned remainBlankCount;
public:
    //! Default Ctor
    GameBoard()
        : loaded(false), rows(0), cols(0), ans(NULL), mask(NULL),
        bombCount(0), flagCount(0), openBlankCount(0), remainBlankCount(0)
    {}
    //! deleted
    GameBoard(const GameBoard&)=delete;
    //! deleted
    GameBoard& operator=(const GameBoard&)=delete;

    // Load Board
    bool isloaded() const {return loaded;}


    // Access board
    /**
     * \brief Access Game Answer
     * \warning No boundary check
     * \return (row, col)-th element of Answer
     */
    char getAnswer(unsigned row, unsigned col) const { return ans[row * cols + col]; }
    /**
     * \brief Access Game Mask
     * \warning No boundary check
     * \return (row, col)-th element of Answer
     */
    char getMask(unsigned row, unsigned col) const { return mask[row * cols + col]; }
    /**
     * @brief Check if (row, col) is on board
     * @return true -> Yes; false -> No
     */
    bool onBoard(unsigned row, unsigned col) const { return row < rows && col < cols; }
    //! Get number of total rows of the board
    unsigned rowSize() const { return rows; }
    //! Get number of total cols of the board
    unsigned colSize() const { return cols; }

    // get count
    unsigned getBombCount() const {return bombCount; }
    unsigned getFlagCount() const {return flagCount; }
    unsigned getOpenBlankCount() const {return openBlankCount; }
    unsigned getRemainBlankCount() const {return remainBlankCount; }

};

#endif // GAMEBOARD_H
