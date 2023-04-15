
#ifndef GUIGAME_H
#define GUIGAME_H
#include <QWidget>
#include "GeneralGameWidget.h"


/**
 * @brief Define the layout of game widget when in Standby mode
 * @details
 * From top to down
 * - infoBox
 * - three print button
 * - board loader (QListWidget + QStackWidget + QPushButton)
 * - StartGame button
 */
class StandbyWidget : public GeneralGameWidget{
    Q_OBJECT

public:
    //! ctor
    StandbyWidget(GameBoard*, QWidget* = nullptr);
    //! deleted copy ctor
    StandbyWidget(const StandbyWidget&) =delete;
    //! deleted assignment
    StandbyWidget& operator=(const StandbyWidget&) =delete;
};

#endif // GUIGAME_H
