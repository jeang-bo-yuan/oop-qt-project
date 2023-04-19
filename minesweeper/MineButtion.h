
#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>

#include <QWidget>


class MineButton : public QPushButton
{
    Q_OBJECT

    const int r;
    const int c;

public:
    MineButton(int r, int c, int w = 30, QWidget* parent = nullptr);

    void mouseReleaseEvent(QMouseEvent* event);

public slots:
    void setText(char txt);

signals:
    void leftClicked(int r, int c);
    void rightClicked(int r, int c);
};

#endif // MINEBUTTON_H
