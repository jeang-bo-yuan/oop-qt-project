#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <stddef.h>
#include <string>

class GameBoard
{
public: //member enumeration type
    enum class Ans : char {
        mine = 'X'
    };
    enum class Mask : char {
        closed = '#',
        flag = 'f',
        quetion = '?'
    };

private:
    bool loaded;
    unsigned rows;
    unsigned cols;
    /**
     * @brief Game Answer
     * 非地雷 : 則顯示周圍9宮格內的炸彈數量(0~8)
     * 地雷 : X
     */
    const char* ans;
    /**
     * @brief Game Board
     * 未開啟的格子:#
     * 已開啟的空白格子:顯示周圍9宮格內的炸彈數量(0~8)
     * 標註棋子:f （不可被LeftClick）
     * 標註問號:? （可被LeftClick）
     */
    char* mask;

    unsigned bombCount;
    unsigned flagCount;
    unsigned openBlankCount; //!< 已開啟
    unsigned remainBlankCount; //!< 未開啟
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
    //! Destructor
    ~GameBoard() { unload(); }

    // Load Board
    bool isloaded() const {return loaded;}
    /**
     * \brief Load from file
     * \return true -> Success; false -> Fail
     */
    bool load(const std::string& file);
    /**
     * \brief unload the board
     * \post free merory of ans and mask, and let isloaded() return false
     */
    void unload();


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
