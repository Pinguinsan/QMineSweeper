#ifndef QMINESWEEPER_GAMECONTROLLER_HPP
#define QMINESWEEPER_GAMECONTROLLER_HPP

#include <QObject>

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <set>
#include <unordered_map>

#include "EventTimer.hpp"
#include "QmsGameState.hpp"
#include "MineCoordinateHash.hpp"

class QmsButton;
class MineCoordinates;
class MainWindow;
class QString;
class QmsGameState;

class GameController : public QObject
{
    Q_OBJECT
public:

    ~GameController() override = default;

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
    void incrementUserMineCount();
    void decrementUserMineCount();
    int userDisplayNumberOfMines() const;
    void setNumberOfMinesRemaining(int userDisplayNumberOfMines);

    ChangeAwareInt *userDisplayNumbersOfMinesDataSource();
    ChangeAwareInt *numbersOfMovesMadeDataSource();
    const std::set<MineCoordinates> &mineCoordinates();
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
    const SteadyEventTimer &playTimer() const;
    void startPlayTimer();
    void stopPlayTimer();
    void resumePlayTimer();
    void pausePlayTimer();

    void startResetIconTimer(unsigned int howLong, const QIcon &icon) const;
    bool coordinatePairExists(const MineCoordinates &coordinatesToCheck) const;
    bool mineInBounds(const MineCoordinates &coordinatesToCheck) const;
    bool mineInBounds(int columnIndex, int rowIndex) const;
    void bindMainWindow(std::shared_ptr<MainWindow> mw);

    void assignAllMines();
    void determineNeighborMineCounts();
    void generateRandomMinePlacement(QmsButton *msb);
    void checkForOtherEmptyMines(QmsButton *msb);
    void clearRandomMinePlacement();

    bool isCornerButton(QmsButton *msb) const;
    bool isEdgeButton(QmsButton *msb) const;
    GameState gameState() const;

    void applyGameState(const QmsGameState &state);

    void setCustomMineRatio(float mineRatio);


    static void initializeInstance(int columnCount, int rowCount);

    SaveGameStateResult saveGame(const QString &filePath);
    void loadGame(const QString &filePath);

    static double DEFAULT_NUMBER_OF_MINES();
    static int GAME_TIMER_INTERVAL();
    static int NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES();
    static int DEFAULT_WINKY_FACE_TIMEOUT();
    static int DEFAULT_CRAZY_FACE_TIMEOUT();
    static int DEFAULT_BIG_SMILEY_FACE_TIMEOUT();
    static int DEFAULT_SLEEPY_FACE_TIMEOUT();
    static int LONG_CLICK_THRESHOLD();
    static int MILLISECOND_DELAY_DIGITS();



public slots:
    void onMineSweeperButtonCreated(std::shared_ptr<QmsButton> msbp);
    void onMineSweeperButtonLeftClicked(QmsButton *msbp);
    void onMineSweeperButtonRightClicked(QmsButton *msbp);
    void onMineSweeperButtonLeftClickReleased(QmsButton *msbp);
    void onMineSweeperButtonRightClickReleased(QmsButton *msbp);
    void onMineSweeperButtonLongLeftClickReleased(QmsButton *msbp);
    void onMineSweeperButtonLongRightClickReleased(QmsButton *msbp);
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
    void numberOfMinesRemainingChanged(int newNumber);
    void numberOfMovesMadeChanged(int newNumber);
    void customMineRatioSet(float mineRatio);
    void loadGameCompleted(LoadGameStateResult loadResult, const QmsGameState &gameState);

private:
    std::shared_ptr<QmsGameState> m_qmsGameState;
    std::shared_ptr<MainWindow> m_mainWindow;

    static const double s_DEFAULT_NUMBER_OF_MINES;
    static const int s_GAME_TIMER_INTERVAL;
    static const int s_NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
    static const int s_DEFAULT_WINKY_FACE_TIMEOUT;
    static const int s_DEFAULT_CRAZY_FACE_TIMEOUT;
    static const int s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT;
    static const int s_DEFAULT_SLEEPY_FACE_TIMEOUT;
    static const int s_LONG_CLICK_THRESHOLD;
    static const int s_MILLISECOND_DISPLAY_DIGITS;


    GameController(int columnCount, int rowCount);
    GameController(const GameController &other) = delete;
    GameController(GameController &&other) = delete;

};

extern GameController *gameController;


#endif //QMINESWEEPER_GAMECONTROLLER_HPP
