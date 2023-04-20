
#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>

#include <QWidget>

/**
 * @brief 採地雷遊戲的格子（for GUI）
 * @details 右鍵和左鍵點擊時會發出不同訊號，而且顯示文字時會有文字美觀
 */
class MineButton : public QPushButton
{
    Q_OBJECT

    const int r;
    const int c;

public:
    /**
     * @brief Constructor
     * @param r - 所在的row
     * @param c - 所在的col
     * @param w - 按鍵的寬度
     * @param parent - parent QWidget
     */
    MineButton(int r, int c, int w = 30, QWidget* parent = nullptr);

    /**
     * @brief mouseReleaseEvent
     * @post 如果以滑鼠左鍵或右鍵點擊，emit leftClicked or rightClicked
     */
    void mouseReleaseEvent(QMouseEvent* event);

public slots:
    //! set text + 美觀
    void setText(char txt);

signals:
    //! 左鍵點擊時發出
    void leftClicked(int r, int c);
    //! 右鍵點擊時發出
    void rightClicked(int r, int c);
};

#endif // MINEBUTTON_H
