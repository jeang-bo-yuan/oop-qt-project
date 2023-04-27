
#ifndef GENERALGAMEWIDGET_H
#define GENERALGAMEWIDGET_H


#include <QWidget>
#include <QGridLayout>
#include <memory>
#include "GameBoard.h"
#include "QT_ResourcePack.h"

/**
 * @brief Define the general layout of the game widget
 * @details
 * From top to down
 * - infoBox
 * - three print buttons (only when QT_DEBUG is defined)
 */
class GeneralGameWidget : public QWidget {
protected:
    const std::shared_ptr<GameBoard> board_p;
    const QString state;
    const std::shared_ptr<QT_ResourcePack> resource_p;
    QVBoxLayout* const vLayout;
    QGridLayout* const infoBox;

    /**
     * @brief constructor
     * @param board_p - pointer to Minesweeper GameBoard
     * @param state - 顯示的Game State
     * @param resource_p - pointer to QT_ResourcePack
     * @param parent - parent QWidget
     */
    GeneralGameWidget(std::shared_ptr<GameBoard> board_p, const QString& state, std::shared_ptr<QT_ResourcePack> resource_p, QWidget* parent = nullptr);
    //! deleted copy ctor
    GeneralGameWidget(const GeneralGameWidget&) =delete;
    //! deleted assignment
    GeneralGameWidget& operator=(const GeneralGameWidget&) =delete;
};

#endif // GENERALGAMEWIDGET_H
