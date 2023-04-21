
#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <stddef.h>
#include <stdexcept>
#include <string>

/**
 * @brief The game board of minesweeper
 * @details
 * ## GameBoard的使用流程
 * construct -> load -> playing (LeftClick or RightClick) -> game over -> unload / destruct
 * @attention
 * GameBoard使用的row, col型別為unsigned，而Qt使用的型別基本上都是int，所以在建立Widget時難免會有轉換上的問題，當row, col超過INT_MAX時可能會在GUI模式下出錯
 */
class GameBoard
{
public: //member enumeration type
    //! icon on answer board
    enum class Ans : char {
        mine = 'X', //!< icon of mine
        random = '?' //!< 出現此符號，代表該格會在第一次 GameBoard::leftClick 後隨機生成
    };
    //! icon on mask board
    enum class Mask : char {
        closed = '#', //!< icon of default slot
        flag = 'f',  //!< icon of flag
        question = '?'  //!< icon of question mark
    };
    //! state of the game board
    enum class GameOver  {
        nope = 0,  //!< game isn't over
        win = 1,  //!< player win the game
        lose = 2 //!< player lose the game
    };

public: // member exception type
    /**
     * @brief The exception class that will be thrown when GameBoard::load failed
     */
    class LoadFailed : public std::runtime_error {
    public:
        LoadFailed(const char* _what) : std::runtime_error(_what) {}
        LoadFailed(const std::string& _what) : std::runtime_error(_what) {}
    };

private:
    bool loaded;
    unsigned rows;
    unsigned cols;
    /**
     * @brief Game Answer
     * @details load from file 或 loadRandomly 後不變
     * - 非地雷 : 則顯示周圍9宮格內的炸彈數量(0~8)
     * - 地雷 : X
     * - 隨機 : ? （第一次leftClick後才會確定）
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
    unsigned flagCount;  //!< mask上的flag數，受 GambBoard::rightClick() 影響
    unsigned openBlankCount; //!< 已開啟的格子數（不含開啟炸彈）
    unsigned remainBlankCount; //!< 未開啟且不是炸彈的格子數
    bool loseGame;
private: // private member function
    void forceSetAns(unsigned row, unsigned col, char c) { ((char*)ans)[row * cols + col] = c;}
    void setMask(unsigned row, unsigned col, char c) { mask[row * cols + col] = c; }
    //! 為 GameBoard::ans 和 GameBoard::mask 分配記憶體，並初始化 GameBoard::mask
    void basicLoad();
    /**
     * \brief 若選擇 Load RandomCount 或 Load RandomRate，則在第一次 GameBoard::leftClick 時會呼叫此函數隨機生成版面
     * \param clickR - leftClick 的列
     * \param clickC - leftClick 的欄
     */
    void loadRandomly(unsigned clickR, unsigned clickC);
    //! 在 GameBoard::ans 上畫上數字，並記算地雷和空格數
    void putNumberOnAns_CountBombAndBlank();

public:
    /**
     * \name Constructor & Destructor
     */
    ///@{
    //! \brief Default Ctor
    //! \details 建立空的踩地雷遊戲盤， GameBoard::isloaded() == false，
    GameBoard()
        : loaded(false), rows(0), cols(0), ans(NULL), mask(NULL),
        bombCount(0), flagCount(0), openBlankCount(0), remainBlankCount(0), loseGame(false)
    {}
    //! deleted copy constructor
    GameBoard(const GameBoard&)=delete;
    //! deleted assignment operator
    GameBoard& operator=(const GameBoard&)=delete;
    //! Destructor, call GameBoard::unload automatically
    ~GameBoard() { unload(); }
    ///@}

    /**
     * \name Load Board
     */
    ///@{
    /**
     * @brief Check if the game board is loaded
     * @return true -> if GameBoard::load is called successfully ; false -> otherwise
     */
    bool isloaded() const {return loaded;}
    /**
     * \brief Load from file
     * \post throw GameBoard::LoadFailed object when load failed
     */
    void load(const std::string& file);
    /**
     * \brief Load with specific amount of mines
     * \pre 0 < bomb < GameBoard::rowSize() * GameBoard::colSize()
     * \post throw GameBoard::LoadFailed object when load failed
     */
    void load(unsigned row, unsigned col, unsigned bomb);
    /**
     * \brief Load with specific possibility to generate bombs
     * \pre 0 < rate < 1
     * \post throw GameBoard::LoadFailed object when load failed
     */
    void load(unsigned row, unsigned col, float rate);
    /**
     * \brief unload the board
     * \post free up merory of ans and mask, and let GameBoard::isloaded return false
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
     * @post ++openBlankCount，--remainBlankCount，如果不是開到地雷；
     *       讓 GameBoard::gameOver 回傳 GameOver::lose ，如果開到地雷
     */
    bool leftClick(unsigned row, unsigned col);
    /**
     * @brief 右鍵點擊(row, col)，在 Mask::flag, Mask::question, Mask::closed 輪替
     * @return true -> Success; false -> Failed
     * @details Fail: 已被開啟, out of range
     * @post ++flagcount if Mask::closed -> Mask::flag;
     *       --flagcount if Mask::flag -> Mask::question
     */
    bool rightClick(unsigned row, unsigned col);
    /**
     * @brief Check if game is over
     * @return GameOver::nope -> not over or not loaded;
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
