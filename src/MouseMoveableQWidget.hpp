#ifndef QMINESWEEPER_MOUSEMOVEABLEQWIDGET_HPP
#define QMINESWEEPER_MOUSEMOVEABLEQWIDGET_HPP

#include <QWidget>
#include <QPair>

class QMouseEvent;

class MouseMoveableQWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MouseMoveableQWidget(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
    void mousePressEvent(QMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void centerAndFitWindow(bool setMinimumSize = false, bool setFixedSize = true);

protected:
    QPair<int, int> calculateXYPlacement();

private:
    QPoint m_mousePressLocation;
};

#endif //QMINESWEEPER_MOUSEMOVEABLEQWIDGET_HPP
