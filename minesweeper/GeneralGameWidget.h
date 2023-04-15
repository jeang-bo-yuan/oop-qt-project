
#ifndef GENERALGAMEWIDGET_H
#define GENERALGAMEWIDGET_H


#include <QWidget>
#include <QGridLayout>
#include <string>
#include "GameBoard.h"

/**
 * @brief Define the general layout of the game widget
 * @details
 * From top to down
 * - infoBox
 * - three print buttons
 */
class GeneralGameWidget : public QWidget {
protected:
    GameBoard* const board_p;
    const std::string state;
    QVBoxLayout* const vLayout;
    QGridLayout* const infoBox;

    //! ctor
    GeneralGameWidget(GameBoard*, const std::string s, QWidget* = nullptr);
    //! deleted copy ctor
    GeneralGameWidget(const GeneralGameWidget&) =delete;
    //! deleted assignment
    GeneralGameWidget& operator=(const GeneralGameWidget&) =delete;
};

#endif // GENERALGAMEWIDGET_H
