
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "GeneralGameWidget.h"
#include "common.h"


GeneralGameWidget::GeneralGameWidget(GameBoard* p, const std::string s,QWidget* parent)
    : QWidget(parent), board_p(p), state(s), vLayout(new QVBoxLayout(this)), infoBox(new QGridLayout(this))
{
// information
    // game state
    infoBox->addWidget(new QLabel("Game State : ", this), 0, 0);
    infoBox->addWidget(new QLabel(state.c_str(), this), 0, 1);
    vLayout->addLayout(infoBox);

// for print
    QHBoxLayout* printBox = new QHBoxLayout(this);
    // print gameboard
    QPushButton* forBoard = new QPushButton("Print GameBoard", this);
    forBoard->setMinimumSize(150, 30);
    QObject::connect(forBoard, &QPushButton::clicked, this, [this]() {printCommand("GameBoard", *(this->board_p), this->state.c_str());});
    // print gameanswer
    QPushButton* forAnswer = new QPushButton("Print GameAnswer", this);
    forAnswer->setMinimumSize(150, 30);
    QObject::connect(forAnswer, &QPushButton::clicked, this, [this]() {printCommand("GameAnswer", *(this->board_p), this->state.c_str());});
    // print gamestate
    QPushButton* forState = new QPushButton("Print GameState", this);
    forState->setMinimumSize(150, 30);
    QObject::connect(forState, &QPushButton::clicked, this, [this]() {printCommand("GameState", *(this->board_p), this->state.c_str());});
    //
    printBox->addWidget(forBoard);
    printBox->addWidget(forAnswer);
    printBox->addWidget(forState);
    vLayout->addLayout(printBox);

    this->setLayout(vLayout);
}

