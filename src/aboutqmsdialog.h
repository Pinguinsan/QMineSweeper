/***********************************************************************
*    aboutqmsdialog.h:                                                 *
*    Custom QDialog for showing information about QMineSweeper         *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an AboutQmsDialog class       *
*    The AboutQmsDialog class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_ABOUTQMSDIALOG_H
#define QMINESWEEPER_ABOUTQMSDIALOG_H

namespace Ui {
    class AboutQmsDialog;
}

#include <QDialog>
#include <memory>

class QDesktopWidget;

class AboutQmsDialog : public QDialog
{
    Q_OBJECT
public:
    AboutQmsDialog();
    ~AboutQmsDialog();
    void centerAndFitWindow(QDesktopWidget *desktopWidget);

signals:
    void aboutToClose();
    void keyPressEvent(QKeyEvent *event);

public:
    void closeEvent(QCloseEvent *ce);

private slots:
    void onLicenseButtonClicked(bool checked);
    void onCloseButtonClicked(bool checked);
private:
    std::unique_ptr<Ui::AboutQmsDialog> m_ui;
    int m_xPlacement;
    int m_yPlacement;

    void calculateXYPlacement(QDesktopWidget *desktopWidget);
};

#endif //QMINESWEEPER_ABOUTQMSWINDOW_H
