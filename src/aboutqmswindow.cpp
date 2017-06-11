/***********************************************************************
*    aboutqmswindow.cpp:                                               *
*    Custom QMainWindow for showing information about QMineSweeper     *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of an AboutQmsWindow class     *
*    The AboutQmsWindow class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "aboutqmswindow.h"
#include <QDesktopWidget>
#include <QRect>

#include <memory>

AboutQmsWindow::AboutQmsWindow() :
    m_xPlacement{0},
    m_yPlacement{0}
{

}

AboutQmsWindow::~AboutQmsWindow()
{

}

/* centerAndFitWindow() : Called whenever code requests the main window to be centered
 * Typically called after resizing the UI on this widget / window */
void AboutQmsWindow::centerAndFitWindow(QDesktopWidget *desktopWidget)
{
    this->calculateXYPlacement(desktopWidget);
    this->move(this->m_xPlacement, this->m_yPlacement);
}

/* calculateXYPlacement() : Checks the currently available screen geometry, and calulates
 * where the widget must be moved to appear on at the center of the users screen */
void AboutQmsWindow::calculateXYPlacement(QDesktopWidget *desktopWidget)
{
    std::unique_ptr<QRect> avail{new QRect{desktopWidget->availableGeometry()}};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
#if defined(__ANDROID__)
    this->m_yPlacement = avail->height() - this->height();
#else
    this->m_yPlacement = (avail->height()/2)-(this->height()/2);
#endif
}


void AboutQmsWindow::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    QMainWindow::closeEvent(ce);
}

