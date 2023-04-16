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

#include "GameBoard.h"
#include "common.h"
#include "GUIGame.h"

/**
 * @brief 開始GUI Game的初始化
 * @param argc
 * @param argv
 * @return QApplication::exec()
 */
int startGUIGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    GameBoard board;

    QScopedPointer<StandbyWidget> standby(new StandbyWidget(&board));
    standby->show();

    return app.exec();
}

// StandbyWidget的實作

StandbyWidget::StandbyWidget(GameBoard* p, QWidget* parent)
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
