/**************************************************//**
 * \file GUIGame.cpp
 * \brief 初始化GUI Game會用到的widget
 * \details 內含startGUIGame 及 StandbyWidget的實作
******************************************************/
#include <QApplication>
#include <QComboBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <iostream>
#include <string>

#include "GameBoard.h"
#include "common.h"
#include "GUIGame.h"
#include "MineButtion.h"

/**
 * @brief 開始GUI Game的初始化
 * @param argc
 * @param argv
 * @return QApplication::exec()
 */
int startGUIGame(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::shared_ptr<GameBoard> board_p = std::make_shared<GameBoard>();
    StandbyWidget* standby = new StandbyWidget(board_p);
    PlayingWidget* playing = new PlayingWidget(board_p);

    standby->show();
    QObject::connect(standby, &StandbyWidget::startGame, playing, &PlayingWidget::initGameBoard);
    QObject::connect(playing, &PlayingWidget::replay, standby, &StandbyWidget::replay);

    return app.exec();
}

// StandbyWidget的實作 ///////////////////////////////////////////////////////////////////////////////////////////////

StandbyWidget::StandbyWidget(std::shared_ptr<GameBoard> p, QWidget* parent)
    : GeneralGameWidget(p, "Standby", parent),
    loader1File(new QLineEdit),
    loader2Row(new QSpinBox), loader2Col(new QSpinBox), loader2Bomb(new QSpinBox),
    loader3Row(new QSpinBox), loader3Col(new QSpinBox), loader3Rate(new QDoubleSpinBox)
{
// loader list + load button
    QHBoxLayout* listAndBut = new QHBoxLayout;

    // loader list
    QComboBox* loaderList = new QComboBox;
    loaderList->addItems({"Load From BoardFile", "Customize Size & Bombs", "Random Rate"});

    // load button
    QPushButton* loadBut = new QPushButton("Load");
    QObject::connect(loadBut, &QPushButton::clicked,
                     this, [this, loaderList] {emit loadButClicked(loaderList->currentIndex());});
    QObject::connect(this, &StandbyWidget::loadButClicked, this, &StandbyWidget::loadBoard);

    //
    listAndBut->addWidget(new QLabel("Loading Mode : "), 1);
    listAndBut->addWidget(loaderList, 4);
    listAndBut->addWidget(loadBut, 1);
    vLayout->addLayout(listAndBut);

// loader setting
    QStackedWidget* settingStack = new QStackedWidget;
    QObject::connect(loaderList, SIGNAL(currentIndexChanged(int)), settingStack, SLOT(setCurrentIndex(int)));

    // for loader1
    {
        QWidget* stack1 = new QWidget;
        QHBoxLayout* sLayout1 = new QHBoxLayout(stack1);

        loader1File->setText("./boards/board1.txt");
        QPushButton* fileBut = new QPushButton("browse...");
        QFileDialog* fileDialog = new QFileDialog(this, "Board File", ".", "*.txt");

        QObject::connect(fileBut, &QPushButton::clicked, fileDialog, [fileDialog]() {fileDialog->exec();});
        QObject::connect(fileDialog, &QFileDialog::fileSelected, loader1File, &QLineEdit::setText);

        sLayout1->addWidget(new QLabel("Board File: "), 1);
        sLayout1->addWidget(loader1File, 3);
        sLayout1->addWidget(fileBut, 1);
        settingStack->addWidget(stack1);
    }


    // for loader2
    {
        QWidget* stack2 = new QWidget;
        QHBoxLayout* sLayout2 = new QHBoxLayout(stack2);
        loader2Row->setMinimum(1);
        loader2Col->setMinimum(1);
        loader2Bomb->setMinimum(1);

        loader2Row->setValue(2);
        loader2Col->setValue(2);

        sLayout2->addWidget(new QLabel("Row: "), 1);
        sLayout2->addWidget(loader2Row, 2);
        sLayout2->addWidget(new QLabel("Col: "), 1);
        sLayout2->addWidget(loader2Col, 2);
        sLayout2->addWidget(new QLabel("Bombs: "), 1);
        sLayout2->addWidget(loader2Bomb, 2);
        settingStack->addWidget(stack2);
    }


    // for loader3
    {
        QWidget* stack3 = new QWidget;
        QHBoxLayout* sLayout3 = new QHBoxLayout(stack3);
        loader3Row->setMinimum(1);
        loader3Col->setMinimum(1);
        loader3Rate->setRange(0, 1);
        loader3Rate->setSingleStep(0.05);

        loader3Row->setValue(2);
        loader3Col->setValue(2);
        loader3Rate->setValue(0.3);

        sLayout3->addWidget(new QLabel("Row: "), 1);
        sLayout3->addWidget(loader3Row, 2);
        sLayout3->addWidget(new QLabel("Col: "), 1);
        sLayout3->addWidget(loader3Col, 2);
        sLayout3->addWidget(new QLabel("Bomb Generating Rate: "), 1);
        sLayout3->addWidget(loader3Rate, 2);
        settingStack->addWidget(stack3);
    }

    // synchronize row and col SpinBox in loader2 & loader3
    {
        QObject::connect(loader2Row, SIGNAL(valueChanged(int)), loader3Row, SLOT(setValue(int)));
        QObject::connect(loader3Row, SIGNAL(valueChanged(int)), loader2Row, SLOT(setValue(int)));
        QObject::connect(loader2Col, SIGNAL(valueChanged(int)), loader3Col, SLOT(setValue(int)));
        QObject::connect(loader3Col, SIGNAL(valueChanged(int)), loader2Col, SLOT(setValue(int)));
    }

    vLayout->addWidget(settingStack);

// startgame button
    {
        QPushButton* startBut = new QPushButton("Start Game");
        QObject::connect(startBut, &QPushButton::clicked, this, [this]() {
            std::cout << "<StartGame> : ";
            if (this->board_p->isloaded()){
                std::cout << "Success" << std::endl;
                this->hide();
                emit this->startGame();
            }
            else {
                std::cout << "Failed" << std::endl;
            }
        });

        QHBoxLayout* startBox = new QHBoxLayout;
        startBox->addStretch(1);
        startBox->addWidget(startBut, 2);
        startBox->addStretch(1);
        vLayout->addLayout(startBox);
    }
}

void StandbyWidget::loadBoard(int loaderIdx) {
    QString arg1, arg2, arg3;
    QString cmd;

    switch (loaderIdx) {
    case 0: // use loader1
        arg1 = loader1File->text();
        cmd = QString("Load Boardfile ") + arg1;
        printCommandSuccessOrNot(qPrintable(cmd), board_p->load(qPrintable(arg1)));
        break;

    case 1: // use loader2
        arg1 = loader2Row->text();
        arg2 = loader2Col->text();
        arg3 = loader2Bomb->text();
        cmd = QString("Load RandomCount ") + arg1 + " " + arg2 + " " + arg3;
        printCommandSuccessOrNot(qPrintable(cmd), board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toUInt()));
        break;

    case 2:
        arg1 = loader3Row->text();
        arg2 = loader3Col->text();
        arg3 = loader3Rate->text();
        cmd = QString("Load RandomRate ") + arg1 + " " + arg2 + " " + arg3;
        printCommandSuccessOrNot(qPrintable(cmd), board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toFloat()));
        break;
    }
}

void StandbyWidget::replay() {
    this->show();
    board_p->unload();
}

// PlayingWidget的實作 //////////////////////////////////////////////////////////////////////////////////////////////

PlayingWidget::PlayingWidget(std::shared_ptr<GameBoard> p, QWidget* parent)
    : GeneralGameWidget(p, "Playing", parent), guiBoard(nullptr)
{
// infobox
    {
        infoBox->addWidget(new QLabel("Bomb Count : "), 1, 0);
        QLabel* bomb = new QLabel(QString::number(board_p->getBombCount()));
        bomb->setObjectName("BOMB");
        infoBox->addWidget(bomb , 1, 1);

        infoBox->addWidget(new QLabel("Flag Count : "), 1, 2);
        QLabel* flag = new QLabel(QString::number(board_p->getFlagCount()));
        flag->setObjectName("FLAG");
        infoBox->addWidget(flag , 1, 3);

        infoBox->addWidget(new QLabel("Open Blank Count : "), 2, 0);
        QLabel* openBlank = new QLabel(QString::number(board_p->getOpenBlankCount()));
        openBlank->setObjectName("OPEN_BLANK");
        infoBox->addWidget(openBlank, 2, 1);

        infoBox->addWidget(new QLabel("Remain Blank Count : "), 2, 2);
        QLabel* remainBlank = new QLabel(QString::number(board_p->getRemainBlankCount()));
        remainBlank->setObjectName("REMAIN_BLANK");
        infoBox->addWidget(remainBlank, 2, 3);
    }

}

void PlayingWidget::initGameBoard() {
    updateInfoBox();

    delete guiBoard;

    guiBoard = new QGridLayout;
    for (size_t r = 0; r < board_p->rowSize(); ++r) {
        for (size_t c = 0; c < board_p->colSize(); ++c) {
            MineButton* button = new MineButton((int)r, (int)c);
            QObject::connect(button, &MineButton::leftClicked, this, &PlayingWidget::openBlock);
            QObject::connect(button, &MineButton::rightClicked, this, &PlayingWidget::flagBlock);

            guiBoard->addWidget(button, (int)r, (int)c);
        }
    }
    guiBoard->setSpacing(0);
    guiBoard->setContentsMargins(0, 0, 0, 0);
    guiBoard->setAlignment(Qt::AlignCenter);

    vLayout->addLayout(guiBoard);

    this->show();
}

void PlayingWidget::updateInfoBox() {
    QLabel* p = this->findChild<QLabel*>("BOMB");
    p->setText(QString::number(board_p->getBombCount()));

    p = this->findChild<QLabel*>("FLAG");
    p->setText(QString::number(board_p->getFlagCount()));

    p = this->findChild<QLabel*>("OPEN_BLANK");
    p->setText(QString::number(board_p->getOpenBlankCount()));

    p = this->findChild<QLabel*>("REMAIN_BLANK");
    p->setText(QString::number(board_p->getRemainBlankCount()));
}

void PlayingWidget::openBlock(int r, int c) {
    std::string cmd = "LeftClick " + std::to_string(r) + " " + std::to_string(c);
    bool status = board_p->leftClick(r, c);
    printCommandSuccessOrNot(cmd, status);

    if (!status)
        return;

    updateInfoBox();
    // draw
    for (size_t r = 0; r < board_p->rowSize(); ++r) {
        for (size_t c = 0; c < board_p->colSize(); ++c) {
            MineButton* button = qobject_cast<MineButton*>(guiBoard->itemAtPosition((int)r, (int)c)->widget());
            button->setText(board_p->getMask((int)r, (int)c));
        }
    }

    // check if game over
    checkIfGameOver();
}

void PlayingWidget::flagBlock(int r, int c) {
    std::string cmd = "RightClick " + std::to_string(r) + " " + std::to_string(c);
    bool status = board_p->rightClick(r, c);
    printCommandSuccessOrNot(cmd, status);

    if (!status)
        return;

    updateInfoBox();
    // draw
    MineButton* button = qobject_cast<MineButton*>(guiBoard->itemAtPosition(r, c)->widget());
    button->setText(board_p->getMask(r, c));

    // check if game over
    checkIfGameOver();
}

void PlayingWidget::checkIfGameOver() {
    QString title, text;

    switch (board_p->gameOver()) {
    case GameBoard::GameOver::win:
        title = "Congratulations!!";
        text = "<b>You Win The Game !!!!!!! </b>";
        break;
    case GameBoard::GameOver::lose:
        title = "Oops...";
        text = "<b>You Lose The Game QAQ  </b>";
        break;
    default:
        return;
    }
    text += "Do you want to replay?";
    QMessageBox* msg = new QMessageBox(QMessageBox::NoIcon, title, text, QMessageBox::Yes | QMessageBox::No, this);

    if (msg->exec() == QMessageBox::Yes) {
        this->hide();
        emit replay();
    }
    else {
        qApp->closeAllWindows();
    }
}
