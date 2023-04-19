
#ifndef GUIGAME_H
#define GUIGAME_H
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include "GeneralGameWidget.h"


/**
 * @brief Define the layout of game widget when in Standby mode
 * @details
 * From top to down
 * - infoBox
 * - three print button
 * - loader list + load button
 * - loader setting (QStackedWidget + QGridLayout)
 * - StartGame button
 */
class StandbyWidget : public GeneralGameWidget{
    Q_OBJECT

    // loader1 (from file)
    QLineEdit* const loader1File;

    // loader2 (specified rows, cols, bombs)
    QSpinBox* const loader2Row;
    QSpinBox* const loader2Col;
    QSpinBox* const loader2Bomb;

    // loader3 (specified rows, cols, rate)
    QSpinBox* const loader3Row;
    QSpinBox* const loader3Col;
    QDoubleSpinBox* const loader3Rate;

public:
    /**
     * @brief constructor
     * @param board_p - pointer to Minesweeper GameBoard
     * @param parent - parent QWidget
     */
    StandbyWidget(std::shared_ptr<GameBoard> board_p, QWidget* parent = nullptr);
    //! deleted copy ctor
    StandbyWidget(const StandbyWidget&) =delete;
    //! deleted assignment
    StandbyWidget& operator=(const StandbyWidget&) =delete;

public slots:
    /**
     * @brief 載入Minesweeper的盤面
     * @param loaderIdx - 欲使用的loader的index
     */
    void loadBoard(int loaderIdx);
    /**
     * @brief 重新開始遊戲
     */
    void replay();
signals:
    /**
     * @brief 當load按鈕被按下時發出
     * @param loaderIdx - 選定的loader的index
     */
    void loadButClicked(int loaderIdx);
    /**
     * @brief 當開始遊戲時發出
     */
    void startGame();
};

/**
 * @brief Define the layout of game widget when in Playing mode
 * @details
 * From top to down
 * - infoBox
 * - three print button
 *
 */
class PlayingWidget : public GeneralGameWidget {
    Q_OBJECT

    //! 包含按鈕
    QGridLayout* guiBoard;

public:
    /**
     * @brief constructor
     * @param p - pointer to minesweeper GameBoard
     * @param parent - parent QWidget
     */
    PlayingWidget(std::shared_ptr<GameBoard> p, QWidget* parent = nullptr);
    //! deleted copy constructor
    PlayingWidget(const PlayingWidget&) =delete;
    //! deleted assignment
    PlayingWidget& operator=(const PlayingWidget&) =delete;

    void checkIfGameOver();

public slots:
    //! used to initiate game board when game start
    void initGameBoard();

    //! update value of infoBox
    void updateInfoBox();

    /**
     * @brief open a block when left click on a block
     * @param r - row of block
     * @param c - col of block
     */
    void openBlock(int r, int c);

    /**
     * @brief put flag when right click on a block
     * @param r - row of block
     * @param c - col of block
     */
    void flagBlock(int r, int c);
signals:
    //! emit if user want to replay
    void replay();
};

#endif // GUIGAME_H
