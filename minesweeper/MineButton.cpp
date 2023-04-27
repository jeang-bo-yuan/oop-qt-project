/************************************************//**
 * \file MineButton.cpp
 * \brief MineButton class
****************************************************/

#include "MineButton.h"
#include <QMouseEvent>
#include "GameBoard.h"

#define BORDER "border: 1px solid black;"

MineButton::MineButton(int r, int c, int w, QWidget* parent)
    : QPushButton(parent), r(r), c(c)
{
    this->setStyleSheet(BORDER "background-color: grey");
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

void MineButton::setText(char txt, QT_ResourcePack* r) {
    switch (txt) {
    // 未開啟
    case (char)GameBoard::Mask::closed:
        QPushButton::setText("");
        setStyleSheet(BORDER "background-color: grey");
        setIcon(QIcon());
        break;
    // 旗子
    case (char)GameBoard::Mask::flag:
        QPushButton::setText("");
        setStyleSheet(BORDER "background-color: blue");
        setIcon(r->getIcon("flag.png"));
        break;
    // 問號
    case (char)GameBoard::Mask::question:
        QPushButton::setText("");
        setStyleSheet(BORDER "background-color: pink");
        setIcon(r->getIcon("question.png"));
        break;
    // 地雷
    case (char)GameBoard::Ans::mine:
        QPushButton::setText("");
        setStyleSheet(BORDER "background-color: red");
        setIcon(r->getIcon("mine.png"));
        break;
    // 數字
    default:
        QString styleSheet(BORDER "background-color: white;" "font-family: impact;");

        if (txt == '0')
            styleSheet += "color: white;";
        else if (txt == '1')
            styleSheet += "color: limegreen;";
        else if (txt == '2')
            styleSheet += "color: darkorange;";
        else
            styleSheet += "color: red;";

        QPushButton::setText(QString(txt));
        setStyleSheet(styleSheet);
        setIcon(QIcon());
        break;
    }
}

