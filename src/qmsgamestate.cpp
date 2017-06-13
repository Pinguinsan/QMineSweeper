/***********************************************************************
*    qmsgamestate.cpp                                                  *
*    Class to hold a saved or loaded game state for QMineSweeper       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsGameState class        *
*    QmsGameState contains information about a saved or loaded game of *
*    QMineSweeper, so a game can be restored at a later date. It also  *
*    includes static methods for serializing a saved game state to a   *
*    file (via Xml), so the games can be quickly loaded in a           *
*    human readable form, rather than using a binary format            *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/


#include "qmsgamestate.h"
#include "qmsstrings.h"
#include "globaldefinitions.h"
#include "qmsbutton.h"

const std::pair<double, double> QmsGameState::s_CELL_TO_MINE_RATIOS{std::make_pair(0.15625, 0.15625)};
const int QmsGameState::s_CELL_TO_MINE_THRESHOLD{82};

QmsGameState::QmsGameState(int columnCount, int rowCount) :
    m_numberOfMines{0},
    m_userDisplayNumberOfMines{0},
    m_initialClickFlag{true},
    m_numberOfColumns{columnCount},
    m_numberOfRows{rowCount},
    m_numberOfMovesMade{0},
    m_gameState{GameState::GAME_INACTIVE},
    m_gameOver{false},
    m_totalButtonCount{this->m_numberOfColumns*this->m_numberOfRows},
    m_unopenedMineCount{this->m_numberOfColumns*this->m_numberOfRows}

{
    using namespace QmsUtilities;
    this->m_numberOfMines = ((this->m_numberOfColumns * this->m_numberOfRows) < this->s_CELL_TO_MINE_THRESHOLD) ?
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.first)) :
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.second));
    this->m_userDisplayNumberOfMines = this->m_numberOfMines;
}


QmsGameState::~QmsGameState()
{

}


int QmsGameState::totalButtonCount() const
{
    return this->m_totalButtonCount;
}

int QmsGameState::unopenedMineCount() const
{
    return this->m_unopenedMineCount;
}

int QmsGameState::userDisplayNumberOfMines() const
{
    return this->m_userDisplayNumberOfMines;
}


void QmsGameState::setNumberOfMinesRemaining(int numberOfMinesRemaining)
{
    this->m_userDisplayNumberOfMines = numberOfMinesRemaining;
}

int QmsGameState::numberOfColumns() const
{
    return this->m_numberOfColumns;
}

int QmsGameState::numberOfMines() const
{
    return this->m_numberOfMines;
}

void QmsGameState::setNumberOfColumns(int numberOfColumns)
{
    this->m_numberOfColumns = numberOfColumns;
}

int QmsGameState::numberOfRows() const
{
    return this->m_numberOfRows;
}

void QmsGameState::setNumberOfRows(int numberOfRows)
{
    this->m_numberOfRows = numberOfRows;
}

bool QmsGameState::initialClickFlag() const
{
    return this->m_initialClickFlag;
}

void QmsGameState::setInitialClickFlag(bool initialClickFlag)
{
    this->m_initialClickFlag = initialClickFlag;
}

void QmsGameState::addMineSweeperButton(int columnIndex, int rowIndex)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    try {
        this->m_mineSweeperButtons.emplace(std::make_pair(MineCoordinates(columnIndex, rowIndex), std::make_shared<QmsButton> (new QmsButton{columnIndex, rowIndex, nullptr})));
    } catch (std::exception &e) {
        LOG_WARNING() << QString{STANDARD_EXCEPTION_CAUGHT_IN_ADD_MINESWEEPER_BUTTON_STRING}.arg(e.what());
    }
}

ButtonContainer &QmsGameState::mineSweeperButtons()
{
    return this->m_mineSweeperButtons;
}

std::shared_ptr<QmsButton> QmsGameState::mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const
{
    return this->m_mineSweeperButtons.at(coordinates);
}

std::shared_ptr<QmsButton> QmsGameState::mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const
{
    return this->mineSweeperButtonAtIndex(MineCoordinates(columnIndex, rowIndex));
}

void QmsGameState::setGameOver(bool gameOver)
{
    this->m_gameState = GameState::GAME_INACTIVE;
    this->m_gameOver = gameOver;
}

bool QmsGameState::gameOver() const
{
    return this->m_gameOver;
}


void QmsGameState::setNumberOfMovesMade(int numberOfMovesMade)
{
    this->m_numberOfMovesMade = numberOfMovesMade;
}

GameState QmsGameState::gameState() const
{
    return this->m_gameState;
}

int QmsGameState::numberOfMovesMade() const
{
    return this->m_numberOfMovesMade;
}

void QmsGameState::incrementNumberOfMovesMade()
{
    this->m_numberOfMovesMade++;
}

void QmsGameState::decrementNumberOfMovesMade()
{
    this->m_numberOfMovesMade--;
}

void QmsGameState::incrementUserMineCountDisplay()
{
    this->m_userDisplayNumberOfMines++;
}

void QmsGameState::decrementUserMineCountDisplay()
{
    this->m_userDisplayNumberOfMines--;
}

static QmsGameState loadFromFile(const QString &filePath)
{
    return QmsGameState{0, 0};
}

static bool saveToFile(const QString &filePath)
{
    return false;
}
