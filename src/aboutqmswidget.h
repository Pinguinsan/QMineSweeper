/***********************************************************************
*    aboutqmswidget.h:                                                 *
*    Custom QWidget for showing information about QMineSweeper         *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an AboutQmsWidget class       *
*    The AboutQmsWidget class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_ABOUTQMSWIDGET_H
#define QMINESWEEPER_ABOUTQMSWIDGET_H

namespace Ui {
    class AboutQmsWidget;
}

#include <memory>

#include "mousemoveableqwidget.h"

class QDesktopWidget;

class AboutQmsWidget : public MouseMoveableQWidget
{
    Q_OBJECT
public:
    AboutQmsWidget(QWidget *parent = nullptr);
    ~AboutQmsWidget();

signals:
    void aboutToClose();
    void keyPressEvent(QKeyEvent *event);

public:
    void closeEvent(QCloseEvent *ce);

private slots:
    void onLicenseButtonClicked(bool checked);
    void onCloseButtonClicked(bool checked);
private:
    std::unique_ptr<Ui::AboutQmsWidget> m_ui;
    int m_licenseHiddenHeight;

    void populateLicenseText();
    void clearLicenseText();
};

#endif //QMINESWEEPER_ABOUTQMSWIDGET_H
