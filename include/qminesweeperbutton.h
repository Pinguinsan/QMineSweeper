/***********************************************************************
*    minesweeperbutton.h:                                              *
*    Custom QPushButton serving as the base for a QMineSweeper button  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the declarations of a custom QPushButton class    *
*    MineSweeperButton is the base for the game, representing          *
*    a single mine field object, which houses all relevant information *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_MINESWEEPERBUTTON_H
#define QMINESWEEPER_MINESWEEPERBUTTON_H

#if defined(__ANDROID__)
    #include <QTouchEvent>
#endif

#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>
#include <QImage>
#include <QTimer>
#include <QIcon>
#include <memory>
#include <string>

#include "qminesweepericons.h"
#include "mainwindow.h"
#include "minecoordinates.h"
#include "qminesweeperutilities.h"
#include "gamecontroller.h"
#include "eventtimer.h"


class QMineSweeperButton : public QPushButton, public std::enable_shared_from_this<QMineSweeperButton>
{
    Q_OBJECT
public:
    QMineSweeperButton() = delete;
    QMineSweeperButton(int columnIndex, int rowIndex, QWidget *parent);
    QMineSweeperButton(const QMineSweeperButton &other);
    QMineSweeperButton(QMineSweeperButton *other);
    QMineSweeperButton(QMineSweeperButton &&other);
    QMineSweeperButton(std::shared_ptr<QMineSweeperButton> &other);

    bool operator==(const QMineSweeperButton &other) const;
    bool eventFilter(QObject *pObject, QEvent *pEvent) override;

    /*Member access functions*/
    int rowIndex() const;
    int columnIndex() const;
    int numberOfSurroundingMines() const;
    bool hasMine() const;
    bool hasFlag() const;
    bool hasQuestionMark() const;
    bool isRevealed() const;
    std::shared_ptr<MineCoordinates> mineCoordinates() const;
    bool isCornerButton() const;
    bool isEdgeButton() const;

    void setHasMine(bool hasMine);
    void setHasFlag(bool hasFlag);
    void setHasQuestionMark(bool hasQuestionMark);
    void setIsRevealed(bool isRevealed);
    void setNumberOfSurroundingMines(int numberOfSurroundingMines);
    void setBlockClicks(bool blockClicks);
    void drawNumberOfSurroundingMines();

signals:
    void leftClicked(std::shared_ptr<QMineSweeperButton> msbp);
    void rightClicked(std::shared_ptr<QMineSweeperButton> msbp);
    void leftClickReleased(std::shared_ptr<QMineSweeperButton> msbp);
    void longLeftClickReleased(std::shared_ptr<QMineSweeperButton> msbp);
    void rightClickReleased(std::shared_ptr<QMineSweeperButton> msbp);
    void longRightClickReleased(std::shared_ptr<QMineSweeperButton> msbp);

protected slots:
    void mousePressEvent(QMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void doInformLongClick();

private:
    /*Private data members regarding mine status*/
    bool m_hasMine;
    bool m_hasFlag;
    bool m_hasQuestionMark;
    bool m_isRevealed;
    int m_numberOfSurroundingMines;
    int m_columnIndex;
    int m_rowIndex;
    bool m_isBeingLongClicked;
    bool m_blockClicks;
    EventTimer m_longClickTimer;
};

#endif //QMINESWEEPER_MINESWEEPERBUTTON_H
