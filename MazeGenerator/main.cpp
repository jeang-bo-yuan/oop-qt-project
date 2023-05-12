/**
 * @file main.cpp
 * @brief 程式的進入點+MazeGenerator的實作
 */
#include "widget.h"

#include <QApplication>
#include <QThread>
#include <stdlib.h>
#undef NDEBUG
#include <assert.h>

/**
 * @brief 主程式
 */
int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}

/**
 * @brief 從gLayout上獲得Field*
 * @param gLayout
 * @param r - 列
 * @param c - 欄
 * @return 指標，nullptr -> 該格不存在
 */
inline Field* getFromGrid(QGridLayout* gLayout, int r, int c) {
    QLayoutItem* p = gLayout->itemAtPosition(r, c);
    if (p == nullptr)
        return nullptr;
    else
        return qobject_cast<Field*>(p->widget());
}

// 實作drawMaze
void MazeGenerator::drawMaze(QGridLayout* gLayout, TaskStack_t& tasks) {
    if (tasks.empty()) return;

    // retrieve the task to do
    const Task type = std::get<0>(tasks.top());
    const Position from = std::get<1>(tasks.top());
    const Position to = std::get<2>(tasks.top());
    tasks.pop();

    // the range of drawing
    QList<Field*> range;
    // 將函式正在處理的範圍框起來
    auto markRange = [&range]() {
        for (Field* p : range) {
            p->setFrameStyle(QFrame::Panel | QFrame::Raised);
            p->repaint();
        }
        QThread::msleep(500);
    };
    // 取消框起來
    auto unmarkRange = [&range]() {
        QThread::msleep(500);
        for (Field* p : range) {
            p->setFrameStyle(QFrame::NoFrame);
            p->repaint();
        }
    };

    // do the real task
    if (type == Task::DRAW_AND_DIVIDE) {
        qDebug() << "Draw and Divide: from " << from.row << ',' << from.col << " to " << to.row << '.' << to.col;
        assert(from.row < to.row && from.col < to.col);
        for (int r = from.row; r <= to.row; ++r) {
            for (int c = from.col; c <= to.col; ++c) {
                range.push_back(getFromGrid(gLayout, r, c));
            }
        }

        // mark
        markRange();

        // only add legal one
        auto addDrawAndDivide = [&tasks](const Position& from, const Position& to) {
            // 如果不是（from在左上、to在右下 且 長、寬皆大於1）
            if (!(from.row < to.row && from.col < to.col))
                return;
            tasks.emplace(Task::DRAW_AND_DIVIDE, from, to);
        };

        const int Height = to.row - from.row + 1;
        const int Width = to.col - from.col + 1;
        //! 長寬都大於2時就不要選牆邊的點
        const bool DontChoosePosOnEdge = Height > 2 && Width > 2;
        Position randPos = {-1, -1};
        if (DontChoosePosOnEdge) {
            randPos.row = from.row + 1 + rand() % (Height - 2);
            randPos.col = from.col + 1 + rand() % (Width - 2);
        } else {
            randPos.row = from.row + rand() % Height;
            randPos.col = from.col + rand() % Width;
        }

        int drawH = (Height == 2 ? false :  //! 高只有2，強制鉛直畫牆
                     Width  == 2 ? true :   //! 寬只有2，強制水平畫牆
                     rand() % 2);
        if (drawH) {
            // draw wall horizontally
            for (int c = from.col; c <= to.col; ++c) {
                if (c == randPos.col) continue;
                Field* p = getFromGrid(gLayout, randPos.row, c);
                p->setBackgroundRole(QPalette::AlternateBase);
                p->repaint();
            }

            // add clear wall task
            bool canClearUp = randPos.row != from.row;
            bool canClearDown = randPos.row != to.row;
            tasks.emplace(Task::CLEAR_WALL_V, randPos, Position{canClearUp, canClearDown});
            // add two region to tasks
            addDrawAndDivide(from, Position{randPos.row - 1, to.col}); // up part
            addDrawAndDivide(Position{randPos.row + 1, from.col}, to); // down part
        }
        else {
            // draw wall vertically
            for (int r = from.row; r <= to.row; ++r) {
                if (r == randPos.row) continue;
                Field* p = getFromGrid(gLayout, r, randPos.col);
                p->setBackgroundRole(QPalette::AlternateBase);
                p->repaint();
            }

            // add clear wall task
            bool canClearLeft = randPos.col != from.col;
            bool canClearRight = randPos.col != to.col;
            tasks.emplace(Task::CLEAR_WALL_H, randPos, Position{canClearLeft, canClearRight});
            // add two region to tasks
            addDrawAndDivide(from, Position{to.row, randPos.col - 1}); // left part
            addDrawAndDivide(Position{from.row, randPos.col + 1}, to); // right part

        }

        // unmark
        unmarkRange();
    }
    else {
        range.push_back(getFromGrid(gLayout, from.row, from.col));
        if (type == Task::CLEAR_WALL_H) {
            qDebug() << "Clear Wall Horizontally: " << from.row << ',' << from.col;
            if (to.row /*可清左*/) range.push_back(getFromGrid(gLayout, from.row, from.col - 1));
            if (to.col /*可清右*/) range.push_back(getFromGrid(gLayout, from.row, from.col + 1));
        }
        else {
            qDebug() << "Clear Wall Vertically: " << from.row << ',' << from.col;
            if (to.row /*可清上*/) range.push_back(getFromGrid(gLayout, from.row - 1, from.col));
            if (to.col /*可清下*/) range.push_back(getFromGrid(gLayout, from.row + 1, from.col));
        }

        // mark
        markRange();

        // clear wall
        for (Field* p : range) {
            p->setBackgroundRole(QPalette::Base);
            p->repaint();
        }

        // unmark
        unmarkRange();
    }
}
