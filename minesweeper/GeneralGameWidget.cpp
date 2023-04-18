/****************************************************//**
 * \file GeneralGameWidget.cpp
 * \brief implement GeneralGameWidget class
********************************************************/
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "GeneralGameWidget.h"
#include "common.h"


GeneralGameWidget::GeneralGameWidget(std::shared_ptr<GameBoard> p, const QString& s,QWidget* parent)
    : QWidget(parent), board_p(p), state(s), vLayout(new QVBoxLayout(this)), infoBox(new QGridLayout)
{
// information
    // game state
    infoBox->addWidget(new QLabel("Game State : "), 0, 0);
    infoBox->addWidget(new QLabel(this->state), 0, 1);
    vLayout->addLayout(infoBox);

// for print
    QHBoxLayout* printBox = new QHBoxLayout;
    // print gameboard
    QPushButton* forBoard = new QPushButton("Print GameBoard");
    forBoard->setMinimumSize(150, 30);
    QObject::connect(forBoard, &QPushButton::clicked, this, [this]() {printCommand("GameBoard", *(this->board_p), qPrintable(this->state));});
    // print gameanswer
    QPushButton* forAnswer = new QPushButton("Print GameAnswer");
    forAnswer->setMinimumSize(150, 30);
    QObject::connect(forAnswer, &QPushButton::clicked, this, [this]() {printCommand("GameAnswer", *(this->board_p), qPrintable(this->state));});
    // print gamestate
    QPushButton* forState = new QPushButton("Print GameState");
    forState->setMinimumSize(150, 30);
    QObject::connect(forState, &QPushButton::clicked, this, [this]() {printCommand("GameState", *(this->board_p), qPrintable(this->state));});
    //
    printBox->addWidget(forBoard);
    printBox->addWidget(forAnswer);
    printBox->addWidget(forState);
    vLayout->addLayout(printBox);
}

