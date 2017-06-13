/***********************************************************************
*    qmsgamestate.h                                                    *
*    Class to hold a saved or loaded game state for QMineSweeper       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declaration of a QmsGameState class           *
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

#ifndef QMINESWEEPER_QMSGAMESTATE_H
#define QMINESWEEPER_QMSGAMESTATE_H

#include <memory>
#include <set>
#include <utility>
#include <unordered_map>

#include "minecoordinatehash.h"

class QString;
class QmsButton;
class MineCoordinates;
class GameController;

using ButtonContainer = std::unordered_map<MineCoordinates, std::shared_ptr<QmsButton>, MineCoordinateHash>;

enum class GameState {
    GAME_ACTIVE,
    GAME_INACTIVE,
    GAME_PAUSED
};

class QmsGameState
{
    friend class GameController;
public:
    QmsGameState(int columnCount, int rowCount);
    QmsGameState(const QmsGameState &) = default;
    QmsGameState(QmsGameState &&) = default;
    virtual ~QmsGameState();

    bool initialClickFlag() const;
    int numberOfColumns() const;
    int numberOfRows() const;
    int numberOfMines() const;
    int numberOfMovesMade() const;
    int unopenedMineCount() const;
    void setNumberOfMovesMade(int numberOfMovesMade);
    void incrementNumberOfMovesMade();
    void decrementNumberOfMovesMade();
    void incrementUserMineCountDisplay();
    void decrementUserMineCountDisplay();
    int userDisplayNumberOfMines() const;
    void setNumberOfMinesRemaining(int userDisplayNumberOfMines);
    ButtonContainer &mineSweeperButtons();
    std::shared_ptr<QmsButton> mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const;
    std::shared_ptr<QmsButton> mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const;
    bool gameOver() const;
    void setNumberOfColumns(int numberOfColumns);
    void setNumberOfRows(int numberOfRows);
    void setInitialClickFlag(bool initialClickFlag);
    void addMineSweeperButton(int columnIndex, int rowIndex);
    void setGameOver(bool gameOver);
    int totalButtonCount() const;

    static QmsGameState loadFromFile(const QString &filePath);
    static bool saveToFile(const QString &filePath);

    GameState gameState() const;
private:
    std::set<std::pair<int, int>> m_mineCoordinates;
    ButtonContainer m_mineSweeperButtons;
    int m_numberOfMines;
    int m_userDisplayNumberOfMines;
    bool m_initialClickFlag;
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_numberOfMovesMade;
    GameState m_gameState;
    bool m_gameOver;
    int m_totalButtonCount;
    int m_unopenedMineCount;

    static const std::pair<double, double> s_CELL_TO_MINE_RATIOS;
    static const int s_CELL_TO_MINE_THRESHOLD;
};

#endif //QMINESWEEPER_QMSGAMESTATE_H
