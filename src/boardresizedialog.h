/***********************************************************************
*    boardresizedialog.h:                                              *
*    Custom QDialog for settings the board dimensions                  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a BoardResizeDialog class     *
*    The BoardResizeDialog is used to change the size of the game board*
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_BOARDRESIZEDIALOG_H
#define QMINESWEEPER_BOARDRESIZEDIALOG_H

namespace Ui {
    class BoardResizeDialog;
}

#include <QDialog>
#include <memory>

class QDesktopWidget;

class BoardResizeDialog : public QDialog
{

    struct ResizeDialogResult {
        int columns;
        int rows;
        DialogCode userAction;
    };

    Q_OBJECT
public:
    BoardResizeDialog();
    ~BoardResizeDialog();
    void centerAndFitWindow(QDesktopWidget *desktopWidget);
    void show(int columns, int rows);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:
    void aboutToClose(int columns, int rows, DialogCode userRequest);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onOkayButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);
    void onPresetBoardSizeActionTriggered(bool checked);
    void onBoardResizeActionClicked();

private:
    std::unique_ptr<Ui::BoardResizeDialog> m_ui;
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_xPlacement;
    int m_yPlacement;
    ResizeDialogResult m_resultToEmit;

    void calculateXYPlacement(QDesktopWidget *desktopWidget);
};

#endif //QMINESWEEPER_BOARDRESIZEDIALOG_H
