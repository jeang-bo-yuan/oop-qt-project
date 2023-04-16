
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
    //! ctor
    StandbyWidget(GameBoard*, QWidget* = nullptr);
    //! deleted copy ctor
    StandbyWidget(const StandbyWidget&) =delete;
    //! deleted assignment
    StandbyWidget& operator=(const StandbyWidget&) =delete;

public slots:
    void loadBoard(int loaderIdx);
signals:
    void loadButClicked(int loaderIdx);
};

#endif // GUIGAME_H
