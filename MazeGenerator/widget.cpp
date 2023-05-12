
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), gLayout(new QGridLayout)
{
    ui->setupUi(this);
    // 設置右方的工具欄
    ui->verticalLayout->setAlignment(Qt::AlignCenter);
    ui->newBoardBut->setFixedWidth(150);
    ui->nextStepBut->setFixedWidth(150);

    // 設定格子的佈局
    gLayout->setSpacing(0);
    gLayout->setAlignment(Qt::AlignCenter);
    ui->board->setLayout(gLayout);
    QPalette myPalette;
    myPalette.setColor(QPalette::Base, QColor(255, 255, 255));  // 空地
    myPalette.setColor(QPalette::AlternateBase, QColor(0, 0, 0)); // 牆
    myPalette.setColor(QPalette::Light, QColor(19, 219, 4));
    myPalette.setColor(QPalette::Dark, QColor(19, 219, 4));
    ui->board->setPalette(myPalette);

    connect(ui->newBoardBut, &QPushButton::clicked, this, &Widget::newBoard);
    connect(ui->nextStepBut, &QPushButton::clicked, this, &Widget::next);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::clear()
{
    QList<Field*> list = ui->board->findChildren<Field*>();
    for (Field* p : list) {
        delete p;
    }
}

void Widget::next()
{
    if (ui->singleStepBox->isChecked())
        MazeGenerator::drawMaze(gLayout, tasks);
    else {
        while(!tasks.empty()) {
            MazeGenerator::drawMaze(gLayout, tasks);
        }
    }
}

void Widget::newBoard()
{
    clear();

    const int row = ui->rowSpin->value(), col = ui->colSpin->value();
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            Field* frame = new Field(r, c);
            gLayout->addWidget(frame, r, c);
        }
    }

    while(!tasks.empty()) {
        tasks.pop();
    }
    tasks.emplace(MazeGenerator::Task::DRAW_AND_DIVIDE
                  , Position{0, 0}
                  , Position{row - 1, col - 1});
    qDebug() << row << '*' << col;
}


