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
#include <QSoundEffect>
#include <QMediaPlayer>
#include <string>
#include <cstring>

#include "command.h"
#include "GUIGame.h"
#include "MineButton.h"

#define DEFAULT_ROW 10
#define DEFAULT_COL 10
#define DEFAULT_BOMB 20
#define DEFAULT_RATE 0.2

#define MAX_ROW 25
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

    // 載入資源
    std::shared_ptr<GameBoard> board_p = std::make_shared<GameBoard>();
    std::shared_ptr<QT_ResourcePack> resource_p = std::make_shared<QT_ResourcePack>(QCoreApplication::applicationDirPath() + "/resource");

    // bgm
    QScopedPointer<QMediaPlayer> bgm(new QMediaPlayer);
    bgm->setMedia(resource_p->getSoundUrl("bgm.mp3"));
    if (!bgm.isNull()) {
        bgm->play();
        QObject::connect(bgm.get(), &QMediaPlayer::stateChanged, bgm.get(), [bgm = bgm.get()](QMediaPlayer::State state) {
            if (state == QMediaPlayer::StoppedState)
                bgm->play();
        });
    }

    // 建立Widget
    QScopedPointer<StandbyWidget> standby(new StandbyWidget(board_p, resource_p));
    QScopedPointer<PlayingWidget> playing(new PlayingWidget(board_p, resource_p));

    standby->show();
    QObject::connect(standby.get(), &StandbyWidget::startGame, playing.get(), &PlayingWidget::initGameBoard);
    QObject::connect(playing.get(), &PlayingWidget::replay, standby.get(), &StandbyWidget::replay);

    return app.exec();
}

// StandbyWidget的實作 ///////////////////////////////////////////////////////////////////////////////////////////////

StandbyWidget::StandbyWidget(std::shared_ptr<GameBoard> p, std::shared_ptr<QT_ResourcePack> r, QWidget* parent)
    : GeneralGameWidget(p, "Standby", r, parent),
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
    loaderList->setObjectName("LOADER_LIST");
    loaderList->addItems({"Load From BoardFile", "Customize Size & Bombs", "Customize Size & Bomb Generating Rate"});

    listAndBut->addWidget(new QLabel("Loading Mode : "), 1);
    listAndBut->addWidget(loaderList, 4);
    vLayout->addLayout(listAndBut);

// loader setting
    QStackedWidget* settingStack = new QStackedWidget;
    QObject::connect(loaderList, SIGNAL(currentIndexChanged(int)), settingStack, SLOT(setCurrentIndex(int)));

    // for loader1
    {
        QWidget* stack1 = new QWidget;
        QHBoxLayout* sLayout1 = new QHBoxLayout(stack1);

        QDir currentFilePath(QDir::current());
        QPushButton* fileBut = new QPushButton("browse...");
        QFileDialog* fileDialog = new QFileDialog(this, "Board File", currentFilePath.absolutePath() + "/resource/boards/", "*.txt");

        QObject::connect(fileBut, &QPushButton::clicked, fileDialog, &QFileDialog::exec);
        QObject::connect(fileDialog, &QFileDialog::fileSelected, loader1File, [this, currentFilePath] (QString file) {
            loader1File->setText(currentFilePath.relativeFilePath(file));
        });

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
    vLayout->addStretch(1);

// startgame button
    {
        QPushButton* startBut = new QPushButton("Start Game");
        QObject::connect(startBut, &QPushButton::clicked, this, &StandbyWidget::loadBoard_and_StartGame);

        QHBoxLayout* startBox = new QHBoxLayout;
        startBox->addStretch(1);
        startBox->addWidget(startBut, 2);
        startBox->addStretch(1);
        vLayout->addLayout(startBox);
    }
}

void StandbyWidget::loadBoard_and_StartGame() {
    int loaderIdx = this->findChild<QComboBox*>("LOADER_LIST")->currentIndex();
    QString arg1, arg2, arg3;
    QString cmd;

    try {
        switch (loaderIdx) {
        case 0: // use loader1
            arg1 = loader1File->text();
            cmd = QString("Load Boardfile ") + arg1;
            board_p->load(qPrintable(arg1));
            break;

        case 1: // use loader2
            arg1 = loader2Row->text();
            arg2 = loader2Col->text();
            arg3 = loader2Bomb->text();
            cmd = QString("Load RandomCount ") + arg1 + " " + arg2 + " " + arg3;
            board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toUInt());
            break;

        case 2: // use loader3
            arg1 = loader3Row->text();
            arg2 = loader3Col->text();
            arg3 = loader3Rate->text();
            cmd = QString("Load RandomRate ") + arg1 + " " + arg2 + " " + arg3;
            board_p->load(arg1.toUInt(), arg2.toUInt(), arg3.toFloat());
            break;
        }
        printCommandSuccessOrNot(qPrintable(cmd), true);

        printCommandSuccessOrNot("StartGame", true);
        this->hide();
        emit this->startGame();
    }
    catch (GameBoard::LoadFailed& ex) {
        QMessageBox::warning(this, "Loading Failed", QString("Can't Load Game Board!!\n") + ex.what());
    }
}

void StandbyWidget::replay() {
    this->show();
    board_p->unload();
}

// PlayingWidget的實作 //////////////////////////////////////////////////////////////////////////////////////////////

PlayingWidget::PlayingWidget(std::shared_ptr<GameBoard> p, std::shared_ptr<QT_ResourcePack> r, QWidget* parent)
    : GeneralGameWidget(p, "Playing", r, parent), guiBoard(new QGridLayout)
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

// GUI Minesweeper Board
    vLayout->addStretch(1);
    vLayout->addLayout(guiBoard);
    vLayout->addStretch(1);
}

void PlayingWidget::initGameBoard() {
    updateInfoBox();

    for (unsigned r = 0; r < board_p->rowSize(); ++r) {
        for (unsigned c = 0; c < board_p->colSize(); ++c) {
            MineButton* button = new MineButton((int)r, (int)c, resource_p);
            QObject::connect(button, &MineButton::leftClicked, this, &PlayingWidget::openBlock);
            QObject::connect(button, &MineButton::rightClicked, this, &PlayingWidget::flagBlock);

            guiBoard->addWidget(button, (int)r, (int)c);
        }
    }
    guiBoard->setSpacing(0);
    guiBoard->setContentsMargins(0, 0, 0, 0);
    guiBoard->setAlignment(Qt::AlignCenter);

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

    QScopedPointer<QMessageBox> msg(new QMessageBox(QMessageBox::NoIcon, title, text, QMessageBox::NoButton, this));
    QPushButton* replayBut = msg->addButton("Replay", QMessageBox::NoRole);
    msg->addButton("Quit", QMessageBox::NoRole);

    msg->exec();
    if (msg->clickedButton() == replayBut) {
        this->hide();

        // delete current board
        for (int r = 0; r < (int)board_p->rowSize(); ++r) {
            for (int c = 0; c < (int)board_p->colSize(); ++c) {
                MineButton* button = qobject_cast<MineButton*>(guiBoard->itemAtPosition(r, c)->widget());
                guiBoard->removeWidget(button);
                delete button;
            }
        }

        printCommandSuccessOrNot("Replay", true);
        emit replay();
    }
    else {
        printCommandSuccessOrNot("Quit", true);
        qApp->closeAllWindows();
    }
}
