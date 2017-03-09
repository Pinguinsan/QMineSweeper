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

#include "qminesweeperbutton.h"


QMineSweeperButton::QMineSweeperButton(int columnIndex, int rowIndex, QWidget *parent) :
    QPushButton{parent},
    m_hasMine{false},
    m_hasFlag{false},
    m_hasQuestionMark{false},
    m_isRevealed{false},
    m_numberOfSurroundingMines{0},
    m_columnIndex{columnIndex},
    m_rowIndex{rowIndex},
    m_isBeingLongClicked{false},
    m_blockClicks{false},
    m_longClickTimer{}
{
   //Row and column validity is checked by GameController
   this->installEventFilter(this);
}                                         


QMineSweeperButton::QMineSweeperButton(QMineSweeperButton *other) :
    QPushButton{static_cast<QWidget*>(other->parent())},
    m_hasMine{other->hasMine()},
    m_hasFlag{other->hasFlag()},
    m_hasQuestionMark{other->hasQuestionMark()},
    m_isRevealed{other->isRevealed()},
    m_numberOfSurroundingMines{other->numberOfSurroundingMines()},
    m_columnIndex{other->columnIndex()},
    m_rowIndex{other->rowIndex()},
    m_isBeingLongClicked{false},
    m_blockClicks{false},
    m_longClickTimer{}
{
    this->installEventFilter(this);
}

QMineSweeperButton::QMineSweeperButton(QMineSweeperButton &&other) :
    QPushButton{static_cast<QWidget*>(std::move(other.parent()))},
    m_hasMine{std::move(other.hasMine())},
    m_hasFlag{std::move(other.hasFlag())},
    m_hasQuestionMark{std::move(other.hasQuestionMark())},
    m_isRevealed{std::move(other.isRevealed())},
    m_numberOfSurroundingMines{std::move(other.numberOfSurroundingMines())},
    m_columnIndex{std::move(other.columnIndex())},
    m_rowIndex{std::move(other.rowIndex())},
    m_isBeingLongClicked{false},
    m_blockClicks{false},
    m_longClickTimer{}
{
   this->installEventFilter(this);
}

QMineSweeperButton::QMineSweeperButton(std::shared_ptr<QMineSweeperButton>& other) :
    QPushButton{static_cast<QWidget*>(other->parent())},
    m_hasMine{std::move(other->m_hasMine)},
    m_hasFlag{std::move(other->m_hasFlag)},
    m_hasQuestionMark{std::move(other->m_hasQuestionMark)},
    m_isRevealed{std::move(other->m_isRevealed)},
    m_numberOfSurroundingMines{std::move(other->m_numberOfSurroundingMines)},
    m_columnIndex{std::move(other->m_columnIndex)},
    m_rowIndex{std::move(other->m_rowIndex)},
    m_isBeingLongClicked{std::move(other->m_isBeingLongClicked)},
    m_blockClicks{std::move(other->m_blockClicks)},
    m_longClickTimer{std::move(other->m_longClickTimer)}
{
    this->installEventFilter(this);
}

bool QMineSweeperButton::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress)  {
        QKeyEvent* pKeyEvent{static_cast<QKeyEvent*>(pEvent)};
        int pressedKey{pKeyEvent->key()};
        if ((pressedKey == Qt::Key_Return) || (pressedKey == Qt::Key_Space)) {
            return true;
        }
    } else  {
        return QObject::eventFilter(pObject, pEvent);
    }
    return true;
}

bool QMineSweeperButton::operator==(const QMineSweeperButton &other) const
{
    return ((this->m_columnIndex == other.columnIndex()) && (this->m_rowIndex == other.rowIndex()));
}

void QMineSweeperButton::mousePressEvent(QMouseEvent *mouseEvent)
{
    if (this->m_blockClicks) {
        return;
    }
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
    this->m_longClickTimer.start();
    QTimer::singleShot(GameController::LONG_CLICK_THRESHOLD(), this, SLOT(doInformLongClick()));
}

void QMineSweeperButton::setBlockClicks(bool blockClicks)
{
    this->m_blockClicks = blockClicks;
}

bool QMineSweeperButton::isBlockingClicks() const
{
    return this->m_blockClicks;
}

void QMineSweeperButton::doInformLongClick()
{
    using namespace QMineSweeperStrings;
    if (this->isDown()) {
        this->setStyleSheet(LONG_CLICKED_MINE_STYLESHEET);
        this->m_isBeingLongClicked = true;
    }
}

void QMineSweeperButton::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    using namespace QMineSweeperStrings;
    if (this->m_blockClicks) {
        return;
    }
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    this->setStyleSheet("");
    this->m_longClickTimer.update();
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            if ((this->m_longClickTimer.totalTime() >= GameController::LONG_CLICK_THRESHOLD()) || (this->m_isBeingLongClicked)) {
                emit (longLeftClickReleased(shared_from_this()));
            } else {
                emit(leftClickReleased(shared_from_this()));
            }
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            if ((this->m_longClickTimer.totalTime() >= GameController::LONG_CLICK_THRESHOLD()) || (this->m_isBeingLongClicked)) {
                emit longLeftClickReleased(shared_from_this());
            } else {
                emit(rightClickReleased(shared_from_this()));
            }
        }
    }
    this->m_isBeingLongClicked = false;
    //this->m_longClickTimer.stop();
}


void QMineSweeperButton::setHasMine(bool hasMine)
{
    this->m_hasMine = hasMine;
}

void QMineSweeperButton::setHasQuestionMark(bool hasQuestionMark)
{
    this->m_hasQuestionMark = hasQuestionMark;
}

void QMineSweeperButton::setHasFlag(bool hasFlag)
{
    this->m_hasFlag = hasFlag;
}

void QMineSweeperButton::setIsRevealed(bool isRevealed)
{
    this->m_isRevealed = isRevealed;
}

bool QMineSweeperButton::isRevealed() const
{
    return this->m_isRevealed;
}

bool QMineSweeperButton::hasMine() const
{
    return this->m_hasMine;
}

bool QMineSweeperButton::hasFlag() const
{
    return m_hasFlag;
}

bool QMineSweeperButton::hasQuestionMark() const
{
    return m_hasQuestionMark;
}

int QMineSweeperButton::columnIndex() const
{
    return this->m_columnIndex;
}

int QMineSweeperButton::rowIndex() const
{
    return this->m_rowIndex;
}

std::shared_ptr<MineCoordinates> QMineSweeperButton::mineCoordinates() const
{
    return std::make_shared<MineCoordinates>(this->m_columnIndex, this->m_rowIndex);
}

int QMineSweeperButton::numberOfSurroundingMines() const
{
    return this->m_numberOfSurroundingMines;
}

void QMineSweeperButton::setNumberOfSurroundingMines(int numberOfSurroundingMines)
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    if (numberOfSurroundingMines < 0) {
        throw std::runtime_error(NUMBER_OF_SURROUND_MINES_NEGATIVE_STRING + toString(numberOfSurroundingMines) + LESS_THAN_ZERO_STRING);
    } else if (numberOfSurroundingMines > 8) {
        throw std::runtime_error(NUMBER_OF_SURROUNDING_MINES_TOO_LARGE_STRING + toString(numberOfSurroundingMines) + GREATER_THAN_8_STRING);
    } else {
        this->m_numberOfSurroundingMines = numberOfSurroundingMines;
    }
}

