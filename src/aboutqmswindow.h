/***********************************************************************
*    aboutqmswindow.h:                                                 *
*    Custom QMainWindow for showing information about QMineSweeper     *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an AboutQmsWindow class       *
*    The AboutQmsWindow class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_ABOUTQMSWINDOW_H
#define QMINESWEEPER_ABOUTQMSWINDOW_H


#include <QMainWindow>

class QDesktopWidget;

class AboutQmsWindow : public QMainWindow
{
    Q_OBJECT
public:
    AboutQmsWindow();
    ~AboutQmsWindow();
    void centerAndFitWindow(QDesktopWidget *desktopWidget);

signals:
    void aboutToClose();
    void keyPressEvent(QKeyEvent *event);

public:
    void closeEvent(QCloseEvent *ce);

private:
    int m_xPlacement;
    int m_yPlacement;

    void calculateXYPlacement(QDesktopWidget *desktopWidget);
};

#endif //QMINESWEEPER_ABOUTQMSWINDOW_H
