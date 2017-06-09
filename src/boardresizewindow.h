/***********************************************************************
*    boardresizewindow.h:                                              *
*    Custom QDialog for settings the board dimensions                  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a BoardResizeWindow class     *
*    The BoardResizeWindow is used to change the size of the game board*
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_BOARDRESIZEWINDOW_H
#define QMINESWEEPER_BOARDRESIZEWINDOW_H

#include <QMainWindow>

class QDesktopWidget;

class BoardResizeWindow : public QMainWindow
{
    Q_OBJECT
public:
    BoardResizeWindow();
    ~BoardResizeWindow();
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

#endif //QMINESWEEPER_BOARDRESIZEWINDOW_H
