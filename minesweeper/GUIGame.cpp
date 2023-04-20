/**************************************************//**
 * \file GUIGame.cpp
 * \brief 初始化GUI Game會用到的widget
 * \details 內含\ref startGUIGame 及 \ref StandbyWidget, \ref PlayingWidget 的實作
******************************************************/
#include <QApplication>
#include <QComboBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QFont>
#include <iostream>
#include <string>
#include <cstring>

#include "common.h"
#include "GUIGame.h"
#include "MineButton.h"

#define DEFAULT_ROW 10
#define DEFAULT_COL 10
#define DEFAULT_BOMB 20
#define DEFAULT_RATE 0.1

#define MAX_ROW 30
#define MIN_ROW 2
#define MAX_COL 30
#define MIN_COL 2
#define MAX_RATE 0.99
#define MIN_RATE 0.01

/**
 * @brief 開始GUI Game的初始化
 * @param argc
 * @param argv
 * @return QApplication::exec()
 */
int startGUIGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QFont defaultFont("nomospace", 10);
    QApplication::setFont(defaultFont);

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
    vLayout->addStretch(1);
    vLayout->addSpacing(20);

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
        loader2Row->setRange(MIN_ROW, MAX_ROW);
        loader2Col->setRange(MIN_COL, MAX_COL);
        loader2Bomb->setMinimum(1);

        loader2Row->setValue(DEFAULT_ROW);
        loader2Col->setValue(DEFAULT_COL);
        loader2Bomb->setValue(DEFAULT_BOMB);

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
        loader3Row->setRange(MIN_ROW, MAX_ROW);
        loader3Col->setRange(MIN_COL, MAX_COL);
        loader3Rate->setRange(MIN_RATE, MAX_RATE);
        loader3Rate->setSingleStep(0.05);

        loader3Row->setValue(DEFAULT_ROW);
        loader3Col->setValue(DEFAULT_COL);
        loader3Rate->setValue(DEFAULT_RATE);

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
                QMessageBox::warning(this, "Can't start the game", "You haven't loaded the game board!");
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
    bool status = true;

    switch (loaderIdx) {
    case 0: // use loader1
        arg1 = loader1File->text();
        cmd = QString("Load Boardfile ") + arg1;
        status = board_p->load(qPrintable(arg1));
        break;

    case 1: // use loader2
        arg1 = loader2Row->text();
        arg2 = loader2Col->text();
        arg3 = loader2Bomb->text();
        cmd = QString("Load RandomCount ") + arg1 + " " + arg2 + " " + arg3;
        status = board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toUInt());
        break;

    case 2:
        arg1 = loader3Row->text();
        arg2 = loader3Col->text();
        arg3 = loader3Rate->text();
        cmd = QString("Load RandomRate ") + arg1 + " " + arg2 + " " + arg3;
        status = board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toFloat());
        break;
    }

    printCommandSuccessOrNot(qPrintable(cmd), status);
    if (!status)
        QMessageBox::warning(this, "Loading Failed", "Can't Load Game Board");
}

void StandbyWidget::replay() {
    this->show();
    board_p->unload();
}

// PlayingWidget的實作 //////////////////////////////////////////////////////////////////////////////////////////////

PlayingWidget::PlayingWidget(std::shared_ptr<GameBoard> p, QWidget* parent)
    : GeneralGameWidget(p, "Playing", parent), guiBoard(nullptr)
{
    vLayout->addStretch(1);

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
    vLayout->addStretch(1);

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
    // draw blocks
    for (unsigned r = 0; r < board_p->rowSize(); ++r) {
        for (unsigned c = 0; c < board_p->colSize(); ++c) {
            char maskTxt = board_p->getMask(r, c);

            // 左鍵點擊不會讓某格轉空格，所以跳過
            if (maskTxt == (char)GameBoard::Mask::closed)
                continue;

            MineButton* button = qobject_cast<MineButton*>(guiBoard->itemAtPosition((int)r, (int)c)->widget());

            // 如果已經開起來了（是數字），不要更新
            if (isdigit(qPrintable(button->text())[0]))
                continue;

            button->setText(maskTxt);
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

    QMessageBox* msg = new QMessageBox(QMessageBox::NoIcon, title, text, QMessageBox::NoButton, this);
    QPushButton* replayBut = msg->addButton("Replay", QMessageBox::NoRole);
    msg->addButton("Quit", QMessageBox::NoRole);

    msg->exec();
    if (msg->clickedButton() == replayBut) {
        this->hide();

        // delete current board
        for (int r = 0; r < (int)board_p->rowSize(); ++r) {
            for (int c = 0; c < (int)board_p->colSize(); ++c) {
                MineButton* button = qobject_cast<MineButton*>(guiBoard->itemAtPosition(r, c)->widget());
                delete button;
            }
        }

        vLayout->removeItem(guiBoard);
        vLayout->removeItem(vLayout->itemAt(vLayout->count() - 1)); // 刪掉最下面的stretch（在initGameBoard被加入）
        delete guiBoard;
        guiBoard = nullptr;

        std::cout << "<Replay> : Success" << std::endl;
        emit replay();
    }
    else {
        std::cout << "<Quit> : Success" << std::endl;
        qApp->closeAllWindows();
    }
}
