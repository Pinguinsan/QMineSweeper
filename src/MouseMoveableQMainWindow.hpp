#ifndef QMINESWEEPER_MOUSEMOVEABLEQMAINWINDOW_HPP
#define QMINESWEEPER_MOUSEMOVEABLEQMAINWINDOW_HPP

#include <QMainWindow>
#include <QPair>

class QMouseEvent;

class MouseMoveableQMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MouseMoveableQMainWindow(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
    void mousePressEvent(QMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void centerAndFitWindow(bool setMinimumSize = false, bool setFixedSize = true);

protected:
    QPair<int, int> calculateXYPlacement();

private:
    QPoint m_mousePressLocation;

};

#endif // MOUSEMOVEABLEQMAINWINDOW_HPP
