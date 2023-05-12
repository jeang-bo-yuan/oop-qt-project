
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <tuple>
#include <stack>
#include <QGridLayout>
#include <QFrame>
#include <QPalette>
#include <QDebug>
#include <QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct Position {
    int row;
    int col;
};

/**
 * @brief 迷宮生成器
 */
class MazeGenerator {
public:
    enum class Task { CLEAR_WALL_V, CLEAR_WALL_H, DRAW_AND_DIVIDE};
    /**
     * @brief 存著drawMaze要做的工作的stack，每個元素皆是一個tuple
     * @details
     * 代表的意義
     * 1. CLEAR_WALL_V + pos + flag
     *      清掉pos上、下的牆壁；flag := { 可清上, 可清下 }
     * 2. CLEAR_WALL_H + pos + flag
     *      清掉pos左、右的牆壁；flag := { 可清左, 可清右 }
     * 3. DRAW_AND_DIVIDE + from + to
     *      對from和to所圍範圍畫迷宮，畫完後將「清牆壁」+「畫迷宮」*(0 ~ 2) 推入stack
     *
     * 清牆壁時不要清超出自己繪畫範圍的點
     */
    typedef std::stack<std::tuple<Task, Position, Position>> TaskStack_t;
    /**
     * @brief 在gLayout上畫迷宮，裡面必須存Field
     * @param gLayout - QGridLayout
     * @param tasks - 為了實現逐步執行的功能，所以用tasks堆疊來模擬遞迴
     * @post 下一層遞迴會儲存在tasks中，後存進去的先執行
     * @details 使用的演算法：recursive division
     */
    static void drawMaze(QGridLayout* gLayout, TaskStack_t& tasks);
};

/**
 * @brief 圖上的格子，點擊後輸出自己的位置
 * @details
 * 不同的backgroundRole和frameStyle代表不同狀態
 * 1. backckgroundRole
 *      - Base ： 空地
 *      - AlternateBase ： 牆壁
 * 2. frameStyle
 *      - NoFrame ： 一般
 *      - Panel | Raised ： 被drawMaze繪畫中
 */
class Field : public QFrame {
    Q_OBJECT
    int r;
    int c;

public:
    Field(int r, int c, QWidget* parent = nullptr) : QFrame(parent), r(r), c(c) {
        this->setLineWidth(2);
        this->setFrameStyle(QFrame::NoFrame);
        this->setBackgroundRole(QPalette::Base);
        this->setAutoFillBackground(true);
        this->setMinimumSize(30, 30);
    }
    virtual void mousePressEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            qDebug() << r << ',' << c;
        }
    }
};

/**
 * @brief 顯示用Widget
 */
class Widget : public QWidget

{
    Q_OBJECT
private:
    Ui::Widget *ui;
    QGridLayout* gLayout;
    MazeGenerator::TaskStack_t tasks;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    //! 清空地圖
    void clear();
    //! 下一步
    void next();
    //! 新地圖
    void newBoard();
};

#endif // WIDGET_H
