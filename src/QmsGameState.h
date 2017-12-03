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
#include <chrono>
#include <map>

#include "MineCoordinateHash.h"
#include "EventTimer.h"
#include "ChangeAwareValue.h"

class QString;
class QmsButton;
class MineCoordinates;
class QXmlStreamWriter;

using ButtonContainer = std::unordered_map<MineCoordinates, std::shared_ptr<QmsButton>, MineCoordinateHash>;
enum class GameState {
    GameActive,
    GameInactive,
    GamePaused
};

enum class SaveGameStateResult {
    Success,
    UnableToDeleteExistingFile,
    UnableToOpenFile,
    UnableToOpenFileToWriteHash,
    UnableToDeleteExistingHashFile
};

enum class LoadGameStateResult {
    Success,
    FileDoesNotExist,
    UnableToOpenFile,
    HashFileDoesNotExist,
    UnableToOpenHashFile,
    HashVerificationFailed
};

class QmsGameState
{
    friend class GameController;
public:
    QmsGameState();
    QmsGameState(int columnCount, int rowCount);
    QmsGameState(const QmsGameState &);
	QmsGameState(QmsGameState &&rhs) noexcept;
	QmsGameState& operator=(const QmsGameState &);
	QmsGameState& operator=(QmsGameState &&rhs);
	~QmsGameState() = default;

    LoadGameStateResult loadGameInPlace(const QString &filePath);
    SaveGameStateResult saveToFile(const QString &filePath);

private:
    std::unique_ptr<SteadyEventTimer> m_playTimer;
    std::set<std::pair<int, int>> m_mineCoordinates;
    ButtonContainer m_mineSweeperButtons;
	int m_numberOfMines;
    ChangeAwareInt m_userDisplayNumberOfMines;
    bool m_initialClickFlag;
    int m_numberOfColumns;
    int m_numberOfRows;
    ChangeAwareInt m_numberOfMovesMade;
    GameState m_gameState;
    bool m_gameOver;
    int m_totalButtonCount;
    int m_unopenedMineCount;
    std::unique_ptr<float> m_customMineRatio;

    static const std::pair<double, double> s_CELL_TO_MINE_RATIOS;
    static const int s_CELL_TO_MINE_THRESHOLD;

    static LoadGameStateResult loadFromFile(const QString &filePath, QmsGameState &targetState);
    void writeQmsButtonToXmlStream(QXmlStreamWriter &writeToFile, const MineCoordinates &coordinates, std::shared_ptr<QmsButton> targetButton);

};

#endif //QMINESWEEPER_QMSGAMESTATE_H
