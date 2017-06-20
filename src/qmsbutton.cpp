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
#include "globaldefinitions.h"

#include <QString>

#if defined (__3D_QMINESWEEPER__)
const int QmsButton::MAXIMUM_NUMBER_OF_SURROUNDING_MINES{27};
#else
const int QmsButton::MAXIMUM_NUMBER_OF_SURROUNDING_MINES{8};
#endif

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

}

std::string QmsButton::toString() const
{
    //return QmsUtilities::TStringFormat("[QmsButtom] ({0}, {1})", this->m_columnIndex, this->m_rowIndex);
    return this->toQString().toStdString();
}

QString QmsButton::toQString() const
{
    return QString{"[QmsButton] (%1, %2)"}.arg(QS_NUMBER(this->m_columnIndex), QS_NUMBER(this->m_rowIndex));
}

bool QmsButton::operator==(const QmsButton &other) const
{
    return ((this->m_columnIndex == other.columnIndex()) && (this->m_rowIndex == other.rowIndex()));
}

void QmsButton::mousePressEvent(QMouseEvent *mouseEvent)
{
    if (this->m_blockClicks) {
        LOG_DEBUG() << QString{"%1 experienced a mousePressEvent, but it's blockClicks property was set, so the event was ignored"}.arg(this->toQString());
        return;
    }
    if (this->isChecked()) {
        this->m_isRevealed = true;
    }
    if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            LOG_DEBUG() << QString{"%1 was left clicked (mouse down only)"}.arg(this->toQString());
            emit(leftClicked(this));
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            LOG_DEBUG() << QString{"%1 was right clicked (mouse down only)"}.arg(this->toQString());
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
                LOG_DEBUG() << QString{"%1 was long left clicked"}.arg(this->toQString());
                emit (longLeftClickReleased(this));
            } else {
                LOG_DEBUG() << QString{"%1 was left clicked"}.arg(this->toQString());
                emit(leftClickReleased(this));
            }
        }
    } else if (mouseEvent->button() == Qt::MouseButton::RightButton) {
        if ((!this->m_isRevealed) && (this->rect().contains(mouseEvent->pos()))) {
            if ((this->m_longClickTimer->totalTime() >= GameController::LONG_CLICK_THRESHOLD()) || (this->m_isBeingLongClicked)) {
                LOG_DEBUG() << QString{"%1 was long right clicked"}.arg(this->toQString());
                emit longRightClickReleased(this);
            } else {
                LOG_DEBUG() << QString{"%1 was right clicked"}.arg(this->toQString());
                emit(rightClickReleased(this));
            }
        }
    }
    this->m_isBeingLongClicked = false;
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
        throw std::runtime_error(TStringFormat("ERROR: In QmsButton::setNumberOfSurroundingMines(int): Argument may not be less than zero ({0} < 0)", numberOfSurroundingMines));
    } else if (numberOfSurroundingMines > 8) {
        throw std::runtime_error(TStringFormat("ERROR: In QmsButton::setNumberOfSurroundingMines(int): Argument may not be greater than maximum number of surrounding mines ({0} > {1})", numberOfSurroundingMines, QmsButton::MAXIMUM_NUMBER_OF_SURROUNDING_MINES));
    } else {
        this->m_numberOfSurroundingMines = numberOfSurroundingMines;
    }
}

QmsButton::~QmsButton()
{

}

