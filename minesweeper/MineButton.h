
#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <memory>
#include "QT_ResourcePack.h"

/**
 * @brief 採地雷遊戲的格子（for GUI）
 * @details 右鍵和左鍵點擊時會發出不同訊號，而且顯示文字時會有文字美觀
 */
class MineButton : public QPushButton
{
    Q_OBJECT

    const int r;
    const int c;
    const std::shared_ptr<QT_ResourcePack> resource_p;

public:
    /**
     * @brief Constructor
     * @param r - 所在的row
     * @param c - 所在的col
     * @param r_p - pointer to QT_ResourcePack
     * @param w - 按鍵的寬度
     * @param parent - parent QWidget
     */
    MineButton(int r, int c, std::shared_ptr<QT_ResourcePack> r_p, int w = 30, QWidget* parent = nullptr);

    /**
     * @brief mouseReleaseEvent
     * @post 如果以滑鼠左鍵或右鍵點擊，emit leftClicked or rightClicked
     */
    void mouseReleaseEvent(QMouseEvent* event);

    /**
     * @brief setText + 美觀
     * @param txt - 文字
     */
    void setText(char txt);

signals:
    //! 左鍵點擊時發出
    void leftClicked(int r, int c);
    //! 右鍵點擊時發出
    void rightClicked(int r, int c);
};

#endif // MINEBUTTON_H
