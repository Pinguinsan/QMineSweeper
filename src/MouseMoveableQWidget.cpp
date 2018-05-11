#include "MouseMoveableQWidget.hpp"

#include <QPoint>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPair>
#include <memory>

MouseMoveableQWidget::MouseMoveableQWidget(QWidget *parent) :
    QWidget{parent},
    m_mousePressLocation{-1, -1}
{
    this->setMouseTracking(true);
}


void MouseMoveableQWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    auto mousePosition = mouseEvent->pos();
    if (this->underMouse()) {
        if (this->m_mousePressLocation == QPoint{-1, -1}) {
            this->m_mousePressLocation = mousePosition;
        }
    }
    return QWidget::mousePressEvent(mouseEvent);
}

void MouseMoveableQWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    this->m_mousePressLocation = QPoint{-1, -1};
    return QWidget::mouseReleaseEvent(mouseEvent);
}

void MouseMoveableQWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if (this->m_mousePressLocation != QPoint{-1, -1}) {
        if (mouseEvent->buttons() & Qt::LeftButton) {
            QPoint diff{mouseEvent->pos() - this->m_mousePressLocation};
            this->move(this->pos() + diff);
        }
    }
    return QWidget::mouseMoveEvent(mouseEvent);
}

void MouseMoveableQWidget::centerAndFitWindow(bool setMinimumSize, bool setFixedSize)
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

QPair<int, int> MouseMoveableQWidget::calculateXYPlacement()
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

