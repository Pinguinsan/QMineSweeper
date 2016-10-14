/***********************************************************************
*    minesweeperbutton.cpp:                                            *
*    Custom QPushButton serving as the base for a QMineSweeper button  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation of a custom QPushButton class  *
*    MineSweeperButton is the base for the game, representing          *
*    a single mine field object, which houses all relevant information *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "minesweeperbutton.h"


MineSweeperButton::MineSweeperButton(int columnIndex, int rowIndex, QWidget *parent) :
    QPushButton{parent},
    m_hasMine{false},
    m_hasFlag{false},
    m_hasQuestionMark{false},
    m_isRevealed{false},
    m_numberOfSurroundingMines{0},
    m_columnIndex{columnIndex},
    m_rowIndex{rowIndex}
{
   //Row and column validity is checked by GameController
   this->installEventFilter(this);
}

MineSweeperButton::MineSweeperButton(MineSweeperButton *other) :
    QPushButton{static_cast<QWidget*>(other->parent())},
    m_hasMine{other->hasMine()},
    m_hasFlag{other->hasFlag()},
    m_hasQuestionMark{other->hasQuestionMark()},
    m_isRevealed{other->isRevealed()},
    m_numberOfSurroundingMines{other->numberOfSurroundingMines()},
    m_columnIndex{other->columnIndex()},
    m_rowIndex{other->rowIndex()}
{
   this->installEventFilter(this);
}

MineSweeperButton::MineSweeperButton(MineSweeperButton &&other) :
    QPushButton{static_cast<QWidget*>(std::move(other.parent()))},
    m_hasMine{std::move(other.hasMine())},
    m_hasFlag{std::move(other.hasFlag())},
    m_hasQuestionMark{std::move(other.hasQuestionMark())},
    m_isRevealed{std::move(other.isRevealed())},
    m_numberOfSurroundingMines{std::move(other.numberOfSurroundingMines())},
    m_columnIndex{std::move(other.columnIndex())},
    m_rowIndex{std::move(other.rowIndex())}
{
   this->installEventFilter(this);
}

MineSweeperButton::MineSweeperButton(std::shared_ptr<MineSweeperButton>& other) :
    QPushButton{static_cast<QWidget*>(other->parent())},
    m_hasMine{other->hasMine()},
    m_hasFlag{other->hasFlag()},
    m_hasQuestionMark{other->hasQuestionMark()},
    m_isRevealed{other->isRevealed()},
    m_numberOfSurroundingMines{other->numberOfSurroundingMines()},
    m_columnIndex{other->columnIndex()},
    m_rowIndex{other->rowIndex()}
{
   this->installEventFilter(this);
}

bool MineSweeperButton::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress)  {
        QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
        int pressedKey = pKeyEvent->key();
        if ((pressedKey == Qt::Key_Return) || (pressedKey == Qt::Key_Space)) {
            return true;
        }
    } else  {
        return QObject::eventFilter(pObject, pEvent);
    }
    return true;
}

bool MineSweeperButton::operator==(const MineSweeperButton &other) const
{
    return ((this->m_columnIndex == other.columnIndex()) && (this->m_rowIndex == other.rowIndex()));
}

void MineSweeperButton::mousePressEvent(QMouseEvent *mouseEvent)
{
    //QPushButton::mousePressEvent(mouseEvent);
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(leftClicked(shared_from_this()));
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(rightClicked(shared_from_this()));
        }
    }
}

void MineSweeperButton::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(leftClickReleased(shared_from_this()));
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(rightClickReleased(shared_from_this()));
        }
    }
}


void MineSweeperButton::setHasMine(bool hasMine)
{
    this->m_hasMine = hasMine;
}

void MineSweeperButton::setHasQuestionMark(bool hasQuestionMark)
{
    this->m_hasQuestionMark = hasQuestionMark;
}

void MineSweeperButton::setHasFlag(bool hasFlag)
{
    this->m_hasFlag = hasFlag;
}

void MineSweeperButton::setIsRevealed(bool isRevealed)
{
    this->m_isRevealed = isRevealed;
}

bool MineSweeperButton::isRevealed() const
{
    return this->m_isRevealed;
}

bool MineSweeperButton::hasMine() const
{
    return this->m_hasMine;
}

bool MineSweeperButton::hasFlag() const
{
    return m_hasFlag;
}

bool MineSweeperButton::hasQuestionMark() const
{
    return m_hasQuestionMark;
}

int MineSweeperButton::columnIndex() const
{
    return this->m_columnIndex;
}

int MineSweeperButton::rowIndex() const
{
    return this->m_rowIndex;
}

std::shared_ptr<MineCoordinates> MineSweeperButton::mineCoordinates() const
{
    return std::make_shared<MineCoordinates>(this->m_columnIndex, this->m_rowIndex);
}

int MineSweeperButton::numberOfSurroundingMines() const
{
    return this->m_numberOfSurroundingMines;
}

void MineSweeperButton::setNumberOfSurroundingMines(int numberOfSurroundingMines)
{
    using namespace QMineSweeperUtilities;
    if (numberOfSurroundingMines < 0) {
        throw std::logic_error("numberOfSurroundingMines cannot be negative (" + toString(numberOfSurroundingMines) + " < 0)");
    } else if (numberOfSurroundingMines > 8) {
        throw std::logic_error("numberOfSurroundMines cannot be greater than 8 (" + toString(numberOfSurroundingMines) + " > 8)");
    } else {
        this->m_numberOfSurroundingMines = numberOfSurroundingMines;
    }
}


void MineSweeperButton::setColumnIndex(int columnIndex)
{
    using namespace QMineSweeperUtilities;
    if (columnIndex < 0) {
        throw std::logic_error("columnIndex cannot be negative (" + toString(columnIndex) + " < 0)");
    }
    this->m_columnIndex = columnIndex;
}


void MineSweeperButton::setRowIndex(int rowIndex)
{
    using namespace QMineSweeperUtilities;
    if (rowIndex < 0) {
        throw std::logic_error("rowIndex cannot be negative (" + toString(rowIndex) + " < 0)");
    }
    this->m_rowIndex = rowIndex;
}


