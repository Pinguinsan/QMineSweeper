/***********************************************************************
*    gamecontroller.h:                                                 *
*    Main controlling logic for the game                               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a GameController class        *
*    A GameController object handles all of the click and pause events *
*    for QMineSweeper, including signals from the main window and from *
*    individual mines. It also holds shared_ptrs to each  MineSweeper  *
*    button, and protects them by only allowing access by coordinates  *
*    (via either a MineCoordinates instance or an x and y coordinate)  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_GAMECONTROLLER_H
#define QMINESWEEPER_GAMECONTROLLER_H

#include <QString>
#include <QMessageBox>
#include <QObject>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include <set>

#include "qminesweeperbutton.h"
#include "minecoordinates.h"
#include "mainwindow.h"
#include "qminesweepericons.h"
#include "minecoordinatehash.h"
#include "qminesweeperutilities.h"

class QMineSweeperButton;
class MineCoordinateHash;
class MineCoordinates;
class MainWindow;

enum class GameState {
    GAME_ACTIVE,
    GAME_INACTIVE,
    GAME_PAUSED
};

class GameController : public QObject
{

    Q_OBJECT
public:

    GameController();
    GameController(const GameController &other) = delete;
    GameController(GameController && other) = delete;

    /*Member access*/
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
    void startResetIconTimer(unsigned int howLong, const QIcon &icon) const;

    std::shared_ptr<QMineSweeperButton> mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const;
    std::shared_ptr<QMineSweeperButton> mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const;
    bool coordinatePairExists(const MineCoordinates &coordinatesToCheck) const;
    bool mineInBounds(const MineCoordinates &coordinatesToCheck) const;
    bool mineInBounds(int columnIndex, int rowIndex) const;
    bool gameOver() const;
    void bindMainWindow(std::shared_ptr<MainWindow> mw);

    void setNumberOfColumns(int numberOfColumns);
    void setNumberOfRows(int numberOfRows);
    void setInitialClickFlag(bool initialClickFlag);
    void addMineSweeperButton(int columnIndex, int rowIndex);
    void setGameOver(bool gameOver);
    void clearRandomMinePlacement();
    void assignAllMines();
    void determineNeighborMineCounts();
    void generateRandomMinePlacement(std::shared_ptr<QMineSweeperButton> msb);
    void checkForOtherEmptyMines(std::shared_ptr<QMineSweeperButton> msb);

    bool isCornerButton(std::shared_ptr<QMineSweeperButton> msb) const;
    bool isEdgeButton(std::shared_ptr<QMineSweeperButton> msb) const;
    GameState gameState() const;
    int totalButtonCount() const;

    static double DEFAULT_NUMBER_OF_MINES();
    static int GAME_TIMER_INTERVAL();
    static std::pair<double, double> CELL_TO_MINE_RATIOS();
    static int CELL_TO_MINE_THRESHOLD();
    static int NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int DEFAULT_WINKY_FACE_TIMEOUT();
    static int DEFAULT_CRAZY_FACE_TIMEOUT();
    static int DEFAULT_BIG_SMILEY_FACE_TIMEOUT();
    static int DEFAULT_SLEEPY_FACE_TIMEOUT();
    static int LONG_CLICK_THRESHOLD();

public slots:
    void onMineSweeperButtonCreated(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonLeftClicked(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonRightClicked(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonLeftClickReleased(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonRightClickReleased(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonLongLeftClickReleased(std::shared_ptr<QMineSweeperButton> msb);
    void onMineSweeperButtonLongRightClickReleased(std::shared_ptr<QMineSweeperButton> msb);
    void onGameReset();
    void onContextMenuActive();
    void onContextMenuInactive();
    void onMineExplosionEventTriggered();
    void onBoardResizeTriggered(int columns, int rows);
    void onGamePaused();
    void onGameResumed();
    void onMineDisplayed();
    void onGameWon();
signals:
    void gameStarted();
    void gamePaused();
    void gameResumed();
    void mineExplosionEvent();
    void readyToBeginNewGame();
    void winEvent();
    void userIsNoLongerIdle();

private:
    std::set<std::pair<int, int>> m_mineCoordinates;
    std::unordered_map<MineCoordinates, std::shared_ptr<QMineSweeperButton>, MineCoordinateHash> m_mineSweeperButtons;
    int m_numberOfMines;
    int m_userDisplayNumberOfMines;
    bool m_initialClickFlag;
    int m_numberOfColumns;
    int m_numberOfRows;
    int m_numberOfMovesMade;
    GameState m_gameState;
    bool m_gameOver;
    std::shared_ptr<MainWindow> m_mwPtr;
    int m_totalButtonCount;
    int m_unopenedMineCount;

    static const double s_DEFAULT_NUMBER_OF_MINES;
    static const int s_GAME_TIMER_INTERVAL;
    static const std::pair<double, double> s_CELL_TO_MINE_RATIOS;
    static const int s_CELL_TO_MINE_THRESHOLD;
    static const int s_NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_DEFAULT_WINKY_FACE_TIMEOUT;
    static const int s_DEFAULT_CRAZY_FACE_TIMEOUT;
    static const int s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT;
    static const int s_DEFAULT_SLEEPY_FACE_TIMEOUT;
    static const int s_LONG_CLICK_THRESHOLD;
};


#endif //QMINESWEEPER_GAMECONTROLLER_H
