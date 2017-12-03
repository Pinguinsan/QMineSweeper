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

#include "MouseMoveableQWidget.h"

class QDesktopWidget;
class QPushButton;

class BoardResizeWidget : public MouseMoveableQWidget
{
    Q_OBJECT
public:
    enum class ResizeWidgetExitCode {
        Accepted,
        Rejected
    };

    struct GameBoardSize {
        float mineRatio;
        int columns;
        int rows;
    };

    struct ResizeWidgetResult {
        GameBoardSize boardSize;
        ResizeWidgetExitCode userAction;
    };

    explicit BoardResizeWidget(QWidget *parent = nullptr);
    ~BoardResizeWidget() override = default;
    void show(int columns, int rows);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:
    void aboutToClose(ResizeWidgetResult result);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void onOkayButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);
    void onBtnBeginnerPresetClicked(bool down);
    void onBtnIntermediatePresetClicked(bool down);
    void onBtnAdvancedPresetClicked(bool down);
    void onBtnExtremePresetClicked(bool down);



private:
    Ui::BoardResizeWidget *m_ui;
    int m_numberOfColumns;
    int m_numberOfRows;
    ResizeWidgetResult m_resultToEmit;

    void onPresetBoardSizeActionTriggered(QPushButton *pressedButton);

    void onBtnIncrementRowsClicked(bool down);
    void onBtnDecrementRowsClicked(bool down);
    void onBtnIncrementColumnsClicked(bool down);
    void onBtnDecrementColumnsClicked(bool down);

    static const std::pair<int, int> BEGINNER_GAME_DIMENSIONS;
    static const std::pair<int, int> INTERMEDIATE_GAME_DIMENSIONS;
    static const std::pair<int, int> ADVANCED_GAME_DIMENSIONS;
    static const std::pair<int, int> EXTREME_GAME_DIMENSIONS;

};

#endif //QMINESWEEPER_BOARDRESIZEWIDGET_H
