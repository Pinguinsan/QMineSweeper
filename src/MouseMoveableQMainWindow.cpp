#include "MouseMoveableQMainWindow.h"

#include <QPoint>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPair>
#include <memory>

MouseMoveableQMainWindow::MouseMoveableQMainWindow(QWidget *parent) :
    QMainWindow{parent},
    m_mousePressLocation{-1, -1}
{
    this->setMouseTracking(true);
}


void MouseMoveableQMainWindow::mousePressEvent(QMouseEvent *mouseEvent)
{
    auto mousePosition = mouseEvent->pos();
    if (this->centralWidget()->underMouse()) {
        if (this->m_mousePressLocation == QPoint{-1, -1}) {
            this->m_mousePressLocation = mousePosition;
        }
    }
    return QMainWindow::mousePressEvent(mouseEvent);
}

void MouseMoveableQMainWindow::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    this->m_mousePressLocation = QPoint{-1, -1};
    return QMainWindow::mouseReleaseEvent(mouseEvent);
}

void MouseMoveableQMainWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if (this->m_mousePressLocation != QPoint{-1, -1}) {
        if (mouseEvent->buttons() & Qt::LeftButton) {
            QPoint diff{mouseEvent->pos() - this->m_mousePressLocation};
            this->move(this->pos() + diff);
        }
    }
    return QMainWindow::mouseMoveEvent(mouseEvent);
}

void MouseMoveableQMainWindow::centerAndFitWindow(bool setMinimumSize, bool setFixedSize)
{
    QSize sizeToSet{};
    if (setMinimumSize) {
        sizeToSet = this->minimumSize();
    } else {
        sizeToSet = this->size();
    }
    if (setFixedSize) {
        this->setFixedSize(sizeToSet);
    } else {
        this->setGeometry(this->x(), this->y(), sizeToSet.width(), sizeToSet.height());
    }
    auto resultPosition = this->calculateXYPlacement();
    this->move(resultPosition.first, resultPosition.second);
}

QPair<int, int> MouseMoveableQMainWindow::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{new QRect{QDesktopWidget{}.availableGeometry()}};
    int x{(avail->width()/2)-(this->width()/2)};
#if defined(__ANDROID__)
    int y{avail->height() - this->height()};
#else
    int y{(avail->height()/2)-(this->height()/2)};
#endif
    return QPair<int, int>{x, y};
}

