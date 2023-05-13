
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), gLayout(new QGridLayout), mazeGen(gLayout, this)
{
    ui->setupUi(this);
    // 設置右方的工具欄
    ui->verticalLayout->setAlignment(Qt::AlignCenter);
    ui->newBoardBut->setFixedWidth(150);
    ui->nextStepBut->setFixedWidth(150);
    ui->stopBut->setFixedWidth(150);
    ui->stopBut->setVisible(false);

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

    // connect
    // on -> single, off -> not single
    connect(ui->singleStepBox, &QCheckBox::toggled, &mazeGen, &MazeGenerator::setSingleShot);
    // 不同文字
    connect(ui->singleStepBox, &QCheckBox::toggled, this, [this](bool on) {
        if (on) {
            ui->nextStepBut->setText("Next Step");
        }
        else {
            ui->nextStepBut->setText("Continue");
        }
    });
    // 格線提示
    connect(ui->hintBox, &QCheckBox::toggled, &mazeGen, &MazeGenerator::setHint);
    // 延遲
    connect(ui->speedSlider, &QSlider::valueChanged, &mazeGen, [p = &mazeGen](int value) {
        // assume value is between [0, 1000]
        float x = value / 100.f;
        // 改變數值變化的幅度，x越大下降越快，x越小上升越快
        // f(x) = 2 x^3 - 30 x^2 + 1000
        float ms = 2.f * x * x * x + -30.f * x * x + 1000.f;
        qDebug() << "ms: " << ms;
        p->setDelayMs(ms);
    });
    // 新盤面
    connect(ui->newBoardBut, &QPushButton::clicked, this, &Widget::newBoard);
    // 下一步
    connect(ui->nextStepBut, &QPushButton::clicked, this, &Widget::next);
    // 停止
    connect(ui->stopBut, &QPushButton::clicked, this, &Widget::stop);
    connect(&mazeGen, &MazeGenerator::finished, this, &Widget::stop);
    // multithreading
    connect(&mazeGen, &MazeGenerator::setFrameStyle, this, [](Field* p, int i) {p->setFrameStyle(i);}, Qt::QueuedConnection);
    connect(&mazeGen, &MazeGenerator::setBackgroundRole, this, [](Field* p, QPalette::ColorRole r) {p->setBackgroundRole(r);}, Qt::QueuedConnection);
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
    mazeGen.init(row, col);
    qDebug() << row << '*' << col;
}

void Widget::next()
{
    if (!ui->singleStepBox->isChecked()) {
        ui->nextStepBut->setVisible(false);
        ui->stopBut->setVisible(true);
    }
    ui->nextStepBut->setDisabled(true);
    ui->newBoardBut->setDisabled(true);
    ui->rowSpin->setDisabled(true);
    ui->colSpin->setDisabled(true);
    ui->singleStepBox->setDisabled(true);
    mazeGen.start();
}

void Widget::stop()
{
    ui->nextStepBut->setVisible(true);
    ui->stopBut->setVisible(false);
    ui->nextStepBut->setDisabled(false);
    ui->newBoardBut->setDisabled(false);
    ui->rowSpin->setDisabled(false);
    ui->colSpin->setDisabled(false);
    ui->singleStepBox->setDisabled(false);
    mazeGen.stop();
}


