
#ifndef GUIGAME_H
#define GUIGAME_H
#include <QWidget>
#include <QLineEdit>
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

    // loader1
    QLineEdit* const loader1File;


public:
    /**
     * @brief constructor
     * @param board_p - pointer to Minesweeper GameBoard
     * @param parent - parent QWidget
     */
    StandbyWidget(GameBoard* board_p, QWidget* parent = nullptr);
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
signals:
    /**
     * @brief 當load按鈕被按下時發出
     * @param loaderIdx - 選定的loader的index
     */
    void loadButClicked(int loaderIdx);
};

#endif // GUIGAME_H
