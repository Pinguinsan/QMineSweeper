/***********************************************************************
*    boardresizewidget.h:                                              *
*    Custom QWidget for settings the board dimensions                  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a BoardResizeWidget class     *
*    The BoardResizeWidget is used to change the size of the game board*
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_BOARDRESIZEWIDGET_H
#define QMINESWEEPER_BOARDRESIZEWIDGET_H

namespace Ui {
    class BoardResizeWidget;
}

#include <memory>
#include "MouseMoveableQWidget.h"

class QDesktopWidget;

class BoardResizeWidget : public MouseMoveableQWidget
{
    Q_OBJECT
public:
    enum class ResizeWidgetExitCode {
        Accepted,
        Rejected
    };

    struct ResizeWidgetResult {
        int columns;
        int rows;
        ResizeWidgetExitCode userAction;
    };

    BoardResizeWidget(QWidget *parent = nullptr);
    ~BoardResizeWidget();
    void show(int columns, int rows);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:
    void aboutToClose(ResizeWidgetResult result);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onOkayButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);
    void onPresetBoardSizeActionTriggered(bool checked);
    void onBoardResizeActionClicked();

private:
    std::unique_ptr<Ui::BoardResizeWidget> m_ui;
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_xPlacement;
    int m_yPlacement;
    ResizeWidgetResult m_resultToEmit;

    void calculateXYPlacement(QDesktopWidget *desktopWidget);
};

#endif //QMINESWEEPER_BOARDRESIZEWIDGET_H
