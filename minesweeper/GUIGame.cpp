#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>

#include "GameBoard.h"
#include "GUIGame.h"



int startGUIGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    GameBoard board;
    board.load(9, 9, 10U);

    QScopedPointer<StandbyWidget> standby(new StandbyWidget(&board));
    standby->show();

    return app.exec();
}

StandbyWidget::StandbyWidget(GameBoard* p, QWidget* parent)
    : GeneralGameWidget(p, "Standby", parent)
{

}
