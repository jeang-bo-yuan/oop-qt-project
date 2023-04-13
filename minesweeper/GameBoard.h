#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <stddef.h>
#include <string>

/**
 * @brief The game board of minesweeper
 */
class GameBoard
{
public: //member enumeration type
    //! icon on answer board
    enum class Ans : char {
        mine = 'X' //!< icon of mine
    };
    //! icon on mask board
    enum class Mask : char {
        closed = '#', //!< icon of default slot
        flag = 'f',  //!< icon of flag
        quetion = '?'  //!< icon of question mark
    };
    //! state of the game board
    enum class GameOver  {
        playing = 0,  //!< game isn't over
        win = 1,  //!< player win the game
        lose = 2 //!< player lose the game
    };

private:
    bool loaded;
    unsigned rows;
    unsigned cols;
    /**
     * @brief Game Answer
     * @details load 後不變
     * - 非地雷 : 則顯示周圍9宮格內的炸彈數量(0~8)
     * - 地雷 : X
     */
    const char* ans;
    /**
     * @brief Game Board
     * @details
     * - 未開啟的格子:#
     * - 已開啟的空白格子:顯示周圍9宮格內的炸彈數量(0~8)
     * - 標註棋子:f （不可被LeftClick）
     * - 標註問號:? （可被LeftClick）
     */
    char* mask;

    unsigned bombCount;  //!< 實際炸彈數，載入後不變
    unsigned flagCount;  //!< mask上的flag數，受GambBoard::rightClick()影響
    unsigned openBlankCount; //!< 已開啟的格子數
    unsigned remainBlankCount; //!< 未開啟且不是炸彈的格子數
    bool loseGame;
private: // private member function
    void forceSetAns(unsigned row, unsigned col, char c) { ((char*)ans)[row * cols + col] = c;}
    void setMask(unsigned row, unsigned col, char c) { mask[row * cols + col] = c; }
    void putNumberOnAns_CountBombAndBlank();

public:
    /**
     * \name Constructor & Destructor
     */
    ///@{
    //! Default Ctor
    GameBoard()
        : loaded(false), rows(0), cols(0), ans(NULL), mask(NULL),
        bombCount(0), flagCount(0), openBlankCount(0), remainBlankCount(0), loseGame(false)
    {}
    //! deleted
    GameBoard(const GameBoard&)=delete;
    //! deleted
    GameBoard& operator=(const GameBoard&)=delete;
    //! Destructor
    ~GameBoard() { unload(); }
    ///@}

    /**
     * \name Load Board
     */
    ///@{
    bool isloaded() const {return loaded;}
    /**
     * \brief Load from file
     * \return true -> Success; false -> Fail
     */
    bool load(const std::string& file);
    /**
     * \brief Load with specific amount of mines
     * \return true -> Success; false -> Fail
     */
    bool load(unsigned row, unsigned col, unsigned bomb);
    /**
     * \brief Load with specific possibility to generate bomb
     * \return true -> Success; false -> Fail
     */
    bool load(unsigned row, unsigned col, float rate);
    /**
     * \brief unload the board
     * \post free merory of ans and mask, and let isloaded() return false
     */
    void unload();
    ///@}


    /**
     * \name Access board
     */
    ///@{
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
    ///@}

    /**
     * \name Playing
     */
    ///@{
    /**
     * @brief 左鍵點擊(row, col)，開啟格子，遇0擴散
     * @return true -> Success; false -> Failed
     * @details Fail: 已被開啟or已插flag, out of range
     * @post --remainBlankCount and ++openBlankCount if open a slot
     *       set losegame if open a mine
     */
    bool leftClick(unsigned row, unsigned col);
    /**
     * @brief 右鍵點擊(row, col)，在flag, quetion, closed輪替
     * @return true -> Success; false -> Failed
     * @details Fail: 已被開啟, out of range
     * @post ++flagcount if closed -> flag;
     *       --flagcount if flag -> question
     */
    bool rightClick(unsigned row, unsigned col);
    /**
     * @brief Check if game is over
     * @return GameOver::playing -> not over or not loaded;
     *         GameOver::win -> win;
     *         GameOver::lose -> lose
     */
    GameOver gameOver() const;
    ///@}

    /**
     * \name Get count
     */
    ///@{
    unsigned getBombCount() const {return bombCount; }
    unsigned getFlagCount() const {return flagCount; }
    unsigned getOpenBlankCount() const {return openBlankCount; }
    unsigned getRemainBlankCount() const {return remainBlankCount; }
    ///@}
};

#endif // GAMEBOARD_H
