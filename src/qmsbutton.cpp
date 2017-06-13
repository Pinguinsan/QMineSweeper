/***********************************************************************
*    minesweeperbutton.cpp:                                            *
*    Custom QPushButton serving as the base for a QMineSweeper button  *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a custom QPushButton class  *
*    MineSweeperButton is the base for the game, representing          *
*    a single mine field object, which houses all relevant information *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qmsbutton.h"
#include "qmsicons.h"
#include "qmsutilities.h"
#include "mainwindow.h"
#include "minecoordinates.h"
#include "gamecontroller.h"
#include "qmsstrings.h"

QmsButton::QmsButton(int columnIndex, int rowIndex, QWidget *parent) :
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
    m_longClickTimer{new SteadyEventTimer{}}
{
    this->initialize();
}

QmsButton::QmsButton(QmsButton *other) :
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
    m_longClickTimer{new SteadyEventTimer{}}
{
    this->initialize();
}

QmsButton::QmsButton(QmsButton &&other) :
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
    m_longClickTimer{new SteadyEventTimer{}}
{
    this->initialize();
}

QmsButton::QmsButton(std::shared_ptr<QmsButton>& other) :
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
    this->initialize();
}

void QmsButton::initialize()
{
    this->installEventFilter(this);
}

/*
int QMineSweeperButton::heightForWidth(int width) const
{
    return width;
}
*/

bool QmsButton::eventFilter(QObject *pObject, QEvent *pEvent)
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
    return false;
}

bool QmsButton::operator==(const QmsButton &other) const
{
    return ((this->m_columnIndex == other.columnIndex()) && (this->m_rowIndex == other.rowIndex()));
}

void QmsButton::mousePressEvent(QMouseEvent *mouseEvent)
{
    if (this->m_blockClicks) {
        return;
    }
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(leftClicked(this));
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            emit(rightClicked(this));
        }
    }
    this->m_longClickTimer->restart();
    QTimer::singleShot(GameController::LONG_CLICK_THRESHOLD(), this, SLOT(doInformLongClick()));
}

void QmsButton::setBlockClicks(bool blockClicks)
{
    this->m_blockClicks = blockClicks;
}

bool QmsButton::isBlockingClicks() const
{
    return this->m_blockClicks;
}

void QmsButton::doInformLongClick()
{
    using namespace QmsStrings;
    if (this->isDown()) {
        this->setStyleSheet(LONG_CLICKED_MINE_STYLESHEET);
        this->m_isBeingLongClicked = true;
    }
}

void QmsButton::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    using namespace QmsStrings;
    if (this->m_blockClicks) {
        return;
    }
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    this->setStyleSheet("");
    this->m_longClickTimer->update();
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            if ((this->m_longClickTimer->totalTime() >= GameController::LONG_CLICK_THRESHOLD()) || (this->m_isBeingLongClicked)) {
                emit (longLeftClickReleased(this));
            } else {
                emit(leftClickReleased(this));
            }
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            if ((this->m_longClickTimer->totalTime() >= GameController::LONG_CLICK_THRESHOLD()) || (this->m_isBeingLongClicked)) {
                emit longLeftClickReleased(this);
            } else {
                emit(rightClickReleased(this));
            }
        }
    }
    this->m_isBeingLongClicked = false;
    //this->m_longClickTimer->stop();
}


void QmsButton::setHasMine(bool hasMine)
{
    this->m_hasMine = hasMine;
}

void QmsButton::setHasQuestionMark(bool hasQuestionMark)
{
    this->m_hasQuestionMark = hasQuestionMark;
}

void QmsButton::setHasFlag(bool hasFlag)
{
    this->m_hasFlag = hasFlag;
}

void QmsButton::setIsRevealed(bool isRevealed)
{
    this->m_isRevealed = isRevealed;
}

bool QmsButton::isRevealed() const
{
    return this->m_isRevealed;
}

bool QmsButton::hasMine() const
{
    return this->m_hasMine;
}

bool QmsButton::hasFlag() const
{
    return m_hasFlag;
}

bool QmsButton::hasQuestionMark() const
{
    return m_hasQuestionMark;
}

int QmsButton::columnIndex() const
{
    return this->m_columnIndex;
}

int QmsButton::rowIndex() const
{
    return this->m_rowIndex;
}

std::shared_ptr<MineCoordinates> QmsButton::mineCoordinates() const
{
    return std::make_shared<MineCoordinates>(this->m_columnIndex, this->m_rowIndex);
}

int QmsButton::numberOfSurroundingMines() const
{
    return this->m_numberOfSurroundingMines;
}

void QmsButton::setNumberOfSurroundingMines(int numberOfSurroundingMines)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (numberOfSurroundingMines < 0) {
        throw std::runtime_error(NUMBER_OF_SURROUND_MINES_NEGATIVE_STRING + toString(numberOfSurroundingMines) + LESS_THAN_ZERO_STRING);
    } else if (numberOfSurroundingMines > 8) {
        throw std::runtime_error(NUMBER_OF_SURROUNDING_MINES_TOO_LARGE_STRING + toString(numberOfSurroundingMines) + GREATER_THAN_8_STRING);
    } else {
        this->m_numberOfSurroundingMines = numberOfSurroundingMines;
    }
}

QmsButton::~QmsButton()
{

}

