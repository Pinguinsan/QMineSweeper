/***********************************************************************
*    qmsbutton.h:                                                      *
*    Custom QPushButton serving as the base for a QMineSweeper button  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a custom QPushButton class    *
*    QmsButton is the base for the game, representing                  *
*    a single mine field object, which houses all relevant information *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMSBUTTON_H
#define QMINESWEEPER_QMSBUTTON_H

#include <QPushButton>
#include <QLabel>

#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include "EventTimer.h"

#include <memory>
#include <string>

class QMouseEvent;
class MineCoordinates;
class QmsIcons;

class QmsButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QmsButton(QWidget *parent = nullptr) = delete;
    explicit QmsButton(int columnIndex, int rowIndex, QWidget *parent = nullptr);
    QmsButton(const QmsButton &rhs);
    QmsButton(QmsButton &&other) noexcept;
    QmsButton &operator=(const QmsButton &rhs);
    QmsButton &operator=(QmsButton &&rhs) noexcept;
    ~QmsButton() override = default;

    bool operator==(const QmsButton &other) const;

    int rowIndex() const;
    int columnIndex() const;
    int numberOfSurroundingMines() const;
    bool hasMine() const;
    bool hasFlag() const;
    bool hasQuestionMark() const;
    bool isRevealed() const;
    std::shared_ptr<MineCoordinates> mineCoordinates() const;

    void setRowIndex(int rowIndex);
    void setColumnIndex(int columnIndex);
    void setCoordinates(const MineCoordinates &coordinates);
    void setHasMine(bool hasMine);
    void setHasFlag(bool hasFlag);
    void setHasQuestionMark(bool hasQuestionMark);
    void setIsRevealed(bool isRevealed);
    void setNumberOfSurroundingMines(int numberOfSurroundingMines);
    void setBlockClicks(bool blockClicks);
    bool isBlockingClicks() const;
    std::string toString() const;
    QString toQString() const;

    void reveal();

    static const int MAXIMUM_NUMBER_OF_SURROUNDING_MINES;

signals:
    void leftClicked(QmsButton *msbp);
    void rightClicked(QmsButton *msbp);
    void leftClickReleased(QmsButton *msbp);
    void longLeftClickReleased(QmsButton *msbp);
    void rightClickReleased(QmsButton *msbp);
    void longRightClickReleased(QmsButton *msbp);

protected slots:
    void mousePressEvent(QMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void doInformLongClick();

private:
    bool m_hasMine;
    bool m_hasFlag;
    bool m_hasQuestionMark;
    bool m_isRevealed;
    int m_numberOfSurroundingMines;
    int m_columnIndex;
    int m_rowIndex;
    bool m_isBeingLongClicked;
    bool m_blockClicks;
    SteadyEventTimer m_longClickTimer;
    void initialize();

};

#endif //QMINESWEEPER_QMSBUTTON_H
