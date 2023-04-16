#include <QApplication>
#include <QComboBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>

#include "GameBoard.h"
#include "common.h"
#include "GUIGame.h"

int startGUIGame(int argc, char* argv[]) {
    QApplication app(argc, argv);
    GameBoard board;

    QScopedPointer<StandbyWidget> standby(new StandbyWidget(&board));
    standby->show();

    return app.exec();
}

StandbyWidget::StandbyWidget(GameBoard* p, QWidget* parent)
    : GeneralGameWidget(p, "Standby", parent), loader1File(new QLineEdit)
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
    listAndBut->addWidget(loaderList, 4);
    listAndBut->addWidget(loadBut, 1);
    vLayout->addLayout(listAndBut);

// loader setting
    QStackedWidget* settingStack = new QStackedWidget;
    QObject::connect(loaderList, SIGNAL(currentIndexChanged(int)), settingStack, SLOT(setCurrentIndex(int)));

    // for loader1
    QWidget* stack1 = new QWidget;
    QHBoxLayout* sLayout1 = new QHBoxLayout(stack1);

    QPushButton* fileBut = new QPushButton("browse...");
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setDirectory("");
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setFilter(QDir::Filter::Readable);

    QObject::connect(fileBut, &QPushButton::clicked, fileDialog, [fileDialog]() {fileDialog->exec();});
    QObject::connect(fileDialog, &QFileDialog::fileSelected, loader1File, &QLineEdit::setText);

    sLayout1->addWidget(new QLabel("Board File: "), 1);
    sLayout1->addWidget(loader1File, 3);
    sLayout1->addWidget(fileBut, 1);
    settingStack->addWidget(stack1);

    //
    vLayout->addWidget(settingStack);
}

void StandbyWidget::loadBoard(int loaderIdx) {
    switch (loaderIdx) {
    case 0: // use loader1
        QString file = loader1File->text();
        QString cmd = QString("Load Boardfile ") + file;
        printCommandSuccessOrNot(qPrintable(cmd), board_p->load(qPrintable(file)));
        break;
    }
}
