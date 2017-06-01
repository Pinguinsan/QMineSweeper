/***********************************************************************
*    boardresizewindow.cpp:                                            *
*    Custom QDialog for settings the board dimensions                  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a BoardResizeWindow class   *
*    The BoardResizeWindow is used to change the size of the game board*
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "boardresizewindow.h"

void BoardResizeWindow::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    QMainWindow::closeEvent(ce);
}
