/************************************************//**
 * \file MineButton.cpp
 * \brief MineButton class
****************************************************/

#include "MineButton.h"
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
    QPushButton::setText(QString(txt == (char)GameBoard::Mask::closed ? '\0' : txt));

    switch (txt) {
    case (char)GameBoard::Mask::closed:
        setStyleSheet(BORDER "background-color: white");
        break;
    case (char)GameBoard::Mask::flag:
        setStyleSheet(BORDER "background-color: blue");
        break;
    case (char)GameBoard::Mask::quetion:
        setStyleSheet(BORDER "background-color: pink");
        break;
    case (char)GameBoard::Ans::mine:
        setStyleSheet(BORDER "background-color: red");
        break;
    default:
        QString styleSheet(BORDER "background-color: white;" "font-family: impact;");

        if (txt == '0')
            styleSheet += "color: limegreen;";
        else if (txt == '1')
            styleSheet += "color: green;";
        else if (txt == '2')
            styleSheet += "color: darkorange;";
        else
            styleSheet += "color: red;";


        setStyleSheet(styleSheet);
        break;
    }
}

