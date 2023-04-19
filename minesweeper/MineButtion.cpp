
#include "MineButtion.h"
#include <QMouseEvent>
#include "GameBoard.h"

#define BORDER "border: 1px solid grey;"

MineButton::MineButton(int r, int c, int w, QWidget* parent)
    : QPushButton(parent), r(r), c(c)
{
    this->setStyleSheet(BORDER "background-color: white");
    this->setFixedSize(w, w);
}

void MineButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() & Qt::RightButton) {
        emit rightClicked(r, c);
    }
    else if (event->button() & Qt::LeftButton) {
        emit leftClicked(r, c);
    }
}

void MineButton::setText(char txt) {
    switch (txt) {
    case (char)GameBoard::Mask::closed:
        QPushButton::setText(QString());
        setStyleSheet(BORDER "background-color: white");
        break;
    case (char)GameBoard::Mask::flag:
        QPushButton::setText(QString(txt));
        setStyleSheet(BORDER "background-color: blue");
        break;
    case (char)GameBoard::Mask::quetion:
        QPushButton::setText(QString(txt));
        setStyleSheet(BORDER "background-color: pink");
        break;
    case (char)GameBoard::Ans::mine:
        QPushButton::setText(QString(txt));
        setStyleSheet(BORDER "background-color: red");
        break;
    default:
        QPushButton::setText(QString(txt));
        setStyleSheet(BORDER "background-color: white");
        break;
    }
}

