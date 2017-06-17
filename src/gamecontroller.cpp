/***********************************************************************
*    gamecontroller.cpp:                                               *
*    Main controlling logic for the game                               *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a GameController class      *
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

#include "gamecontroller.h"

#include <QString>
#include <QMessageBox>

#include <sstream>

#include "qmsbutton.h"
#include "minecoordinates.h"
#include "mainwindow.h"
#include "qmsicons.h"
#include "qmsutilities.h"
#include "qmsstrings.h"
#include "globaldefinitions.h"

const double GameController::s_DEFAULT_NUMBER_OF_MINES{81.0};
const int GameController::s_GAME_TIMER_INTERVAL{100};
const int GameController::s_NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{8};
const int GameController::s_EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{5};
const int GameController::s_CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{3};
const int GameController::s_DEFAULT_WINKY_FACE_TIMEOUT{250};
const int GameController::s_DEFAULT_CRAZY_FACE_TIMEOUT{250};
const int GameController::s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT{500};
const int GameController::s_DEFAULT_SLEEPY_FACE_TIMEOUT{15000};
const int GameController::s_LONG_CLICK_THRESHOLD{250};
const int GameController::s_MILLISECOND_DISPLAY_DIGITS{1};

GameController::GameController(int columnCount, int rowCount) :
    m_qmsGameState{std::make_shared<QmsGameState>(columnCount, rowCount)},
    m_mainWindow{nullptr}
{
    this->connect(this, &GameController::gamePaused, this, &GameController::onGamePaused);
}

void GameController::onMineSweeperButtonCreated(std::shared_ptr<QmsButton> msb)
{
    this->connect(msb.get(), &QmsButton::leftClicked, this, &GameController::onMineSweeperButtonLeftClicked);
    this->connect(msb.get(), &QmsButton::rightClicked, this, &GameController::onMineSweeperButtonRightClicked);
    this->connect(msb.get(), &QmsButton::leftClickReleased, this, &GameController::onMineSweeperButtonLeftClickReleased);
    this->connect(msb.get(), &QmsButton::rightClickReleased, this, &GameController::onMineSweeperButtonRightClickReleased);
    this->connect(msb.get(), &QmsButton::longLeftClickReleased, this, &GameController::onMineSweeperButtonLongLeftClickReleased);
    this->connect(msb.get(), &QmsButton::longRightClickReleased, this, &GameController::onMineSweeperButtonLongLeftClickReleased);
}


int GameController::totalButtonCount() const
{
    return this->m_qmsGameState->m_totalButtonCount;
}

int GameController::unopenedMineCount() const
{
    return this->m_qmsGameState->m_unopenedMineCount;
}

void GameController::onBoardResizeTriggered(int columns, int rows)
{
    using namespace QmsUtilities;
    this->m_qmsGameState->m_numberOfColumns = columns;
    this->m_qmsGameState->m_numberOfRows = rows;
    this->m_qmsGameState->m_numberOfMines = ((this->m_qmsGameState->m_numberOfColumns * this->m_qmsGameState->m_numberOfRows) < QmsGameState::s_CELL_TO_MINE_THRESHOLD) ?
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_qmsGameState->m_numberOfColumns) * static_cast<double>(this->m_qmsGameState->m_numberOfRows) * QmsGameState::s_CELL_TO_MINE_RATIOS.first)) :
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_qmsGameState->m_numberOfColumns) * static_cast<double>(this->m_qmsGameState->m_numberOfRows) * QmsGameState::s_CELL_TO_MINE_RATIOS.second));
    this->m_qmsGameState->m_userDisplayNumberOfMines = this->m_qmsGameState->m_numberOfMines;
    this->m_qmsGameState->m_gameState = GameState::GameInactive;
    this->m_qmsGameState->m_mineCoordinates.clear();
    this->m_qmsGameState->m_mineSweeperButtons.clear();
    this->m_qmsGameState->m_initialClickFlag = true;
    this->m_qmsGameState->m_totalButtonCount = this->m_qmsGameState->m_numberOfColumns * this->m_qmsGameState->m_numberOfRows;
    this->m_qmsGameState->m_unopenedMineCount = this->m_qmsGameState->m_totalButtonCount;
    emit(readyToBeginNewGame());
}

void GameController::onGamePaused()
{
    if ((!this->m_qmsGameState->m_initialClickFlag) && (this->m_qmsGameState->m_gameState == GameState::GameActive)) {
        this->m_qmsGameState->m_gameState = GameState::GamePaused;
        LOG_INFO() << "Game paused";
    }
}

void GameController::onGameResumed()
{
    if ((!this->m_qmsGameState->m_initialClickFlag) && (this->m_qmsGameState->m_gameState == GameState::GamePaused)) {
        this->m_qmsGameState->m_gameState = GameState::GameActive;
        LOG_INFO() << "Game resumed";
    }
}

int GameController::userDisplayNumberOfMines() const
{
    return this->m_qmsGameState->m_userDisplayNumberOfMines;
}

void GameController::onMineExplosionEventTriggered()
{
    this->m_qmsGameState->m_gameState = GameState::GameInactive;
    for (auto &it : this->m_qmsGameState->m_mineSweeperButtons) {
        it.second->setBlockClicks(true);
    }
    LOG_INFO() << "Mine explosion event triggered (game over)";
}

void GameController::setNumberOfMinesRemaining(int numberOfMinesRemaining)
{
    this->m_qmsGameState->m_userDisplayNumberOfMines = numberOfMinesRemaining;
    emit (numberOfMinesRemainingChanged(this->m_qmsGameState->m_userDisplayNumberOfMines));
}

int GameController::numberOfColumns() const
{
    return this->m_qmsGameState->m_numberOfColumns;
}

int GameController::numberOfMines() const
{
    return this->m_qmsGameState->m_numberOfMines;
}

void GameController::setNumberOfColumns(int numberOfColumns)
{
    this->m_qmsGameState->m_numberOfColumns = numberOfColumns;
}

int GameController::numberOfRows() const
{
    return this->m_qmsGameState->m_numberOfRows;
}

void GameController::setNumberOfRows(int numberOfRows)
{
    this->m_qmsGameState->m_numberOfRows = numberOfRows;
}

bool GameController::initialClickFlag() const
{
    return this->m_qmsGameState->m_initialClickFlag;
}

void GameController::setInitialClickFlag(bool initialClickFlag)
{
    this->m_qmsGameState->m_initialClickFlag = initialClickFlag;
}

void GameController::addMineSweeperButton(int columnIndex, int rowIndex)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    try {
        this->m_qmsGameState->m_mineSweeperButtons.emplace(std::make_pair(MineCoordinates(columnIndex, rowIndex), std::make_shared<QmsButton> (new QmsButton{columnIndex, rowIndex, nullptr})));
        LOG_DEBUG() << QString{"Added minesweeper button at (%1, %2)"}.arg(QS_NUMBER(columnIndex), QS_NUMBER(rowIndex));
    } catch (std::exception &e) {
        LOG_WARNING() << QString{STANDARD_EXCEPTION_CAUGHT_IN_ADD_MINESWEEPER_BUTTON_STRING}.arg(e.what());
    }
}

ButtonContainer &GameController::mineSweeperButtons() 
{
    return this->m_qmsGameState->m_mineSweeperButtons;
}

std::set<std::pair<int, int>> &GameController::mineCoordinates()
{
    return this->m_qmsGameState->m_mineCoordinates;
}

const SteadyEventTimer &GameController::playTimer() const
{
    this->m_qmsGameState->m_playTimer->update();
    return *this->m_qmsGameState->m_playTimer;
}

void GameController::startPlayTimer()
{
    this->m_qmsGameState->m_playTimer->start();
}

void GameController::stopPlayTimer()
{
    this->m_qmsGameState->m_playTimer->stop();
}

void GameController::resumePlayTimer()
{
    this->m_qmsGameState->m_playTimer->resume();
}

void GameController::pausePlayTimer()
{
    this->m_qmsGameState->m_playTimer->pause();
}

std::shared_ptr<QmsButton> GameController::mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const
{
    using namespace QmsStrings;
    if (this->mineInBounds(coordinates) && (this->m_qmsGameState->m_mineSweeperButtons.find(coordinates) != this->m_qmsGameState->m_mineSweeperButtons.end())) {
        return this->m_qmsGameState->m_mineSweeperButtons.at(coordinates);
    } else {
        throw std::runtime_error(GENERIC_ERROR_MESSAGE);
    }
}

std::shared_ptr<QmsButton> GameController::mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const
{
    return this->mineSweeperButtonAtIndex(MineCoordinates(columnIndex, rowIndex));
}

void GameController::startResetIconTimer(unsigned int howLong, const QIcon &icon) const
{
    this->m_mainWindow->setResetButtonIcon(icon);
    QTimer::singleShot(static_cast<int>(howLong), this->m_mainWindow.get(), SLOT(resetResetButtonIcon()));
}


bool GameController::isCornerButton(QmsButton *msbp) const
{
    return (((msbp->columnIndex() == 0) && (msbp->rowIndex() == 0)) ||
            ((msbp->columnIndex() == 0) && (msbp->rowIndex() == this->m_qmsGameState->m_numberOfRows - 1)) ||
            ((msbp->columnIndex() == this->m_qmsGameState->m_numberOfColumns - 1) && (msbp->rowIndex() == 0)) ||
            ((msbp->columnIndex() == this->m_qmsGameState->m_numberOfColumns - 1) && (msbp->rowIndex() == this->m_qmsGameState->m_numberOfRows - 1)));
}

bool GameController::isEdgeButton(QmsButton *msbp) const
{
    return ((msbp->columnIndex() == 0) ||
            (msbp->columnIndex() == this->m_qmsGameState->m_numberOfColumns - 1) ||
            (msbp->rowIndex() == 0) ||
            (msbp->rowIndex() == this->m_qmsGameState->m_numberOfRows - 1));
}


void GameController::generateRandomMinePlacement(QmsButton *msbp)
{
    using namespace QmsUtilities;
    MineCoordinates potentialMineCoordinates{0,0};
    while (this->m_qmsGameState->m_mineCoordinates.size() < static_cast<unsigned int>(this->m_qmsGameState->m_numberOfMines)) {
        potentialMineCoordinates = MineCoordinates{randomBetween(0, this->m_qmsGameState->m_numberOfColumns), randomBetween(0, this->m_qmsGameState->m_numberOfRows)};
        if (potentialMineCoordinates == *(msbp->mineCoordinates().get())) {
            continue;
        } else {
            this->m_qmsGameState->m_mineCoordinates.emplace(potentialMineCoordinates.toStdPair());
        }
    }
}

SaveGameStateResult GameController::saveGame(const QString &filePath)
{
    return this->m_qmsGameState->saveToFile(filePath);
}

LoadGameStateResult GameController::loadGame(const QString &filePath)
{
    auto result = this->m_qmsGameState->loadGameInPlace(filePath);
    if (result == LoadGameStateResult::Success) {
        //Do stuff
    }
    return result;
}

void GameController::clearRandomMinePlacement()
{
    this->m_qmsGameState->m_mineCoordinates.clear();
}

void GameController::onGameReset()
{
    for (std::pair<const MineCoordinates, std::shared_ptr<QmsButton>> msbp : this->m_qmsGameState->m_mineSweeperButtons) {
        msbp.second->setHasFlag(false);
        msbp.second->setHasQuestionMark(false);
        msbp.second->setHasMine(false);
        msbp.second->setIsRevealed(false);
        msbp.second->setNumberOfSurroundingMines(0);
        msbp.second->setBlockClicks(false);
    }
    clearRandomMinePlacement();
    this->m_qmsGameState->m_initialClickFlag = true;
    this->m_qmsGameState->m_gameOver = false;
    this->m_qmsGameState->m_userDisplayNumberOfMines = this->m_qmsGameState->m_numberOfMines;
    this->m_qmsGameState->m_gameState = GameState::GameInactive;
    this->m_qmsGameState->m_numberOfMovesMade = 0;
    this->m_qmsGameState->m_unopenedMineCount = this->m_qmsGameState->m_numberOfColumns*this->m_qmsGameState->m_numberOfRows;
}

void GameController::setGameOver(bool gameOver)
{
    this->m_qmsGameState->m_gameState = GameState::GameInactive;
    this->m_qmsGameState->m_gameOver = gameOver;
}

bool GameController::gameOver() const
{
    return this->m_qmsGameState->m_gameOver;
}


bool GameController::coordinatePairExists(const MineCoordinates &coordinatesToCheck) const
{
    for (const auto &mc : this->m_qmsGameState->m_mineCoordinates) {
        if (coordinatesToCheck == mc) {
            return true;
        }
    }
    return false;
}

void GameController::setNumberOfMovesMade(int numberOfMovesMade)
{
    this->m_qmsGameState->m_numberOfMovesMade = numberOfMovesMade;
}


void GameController::assignAllMines()
{
    using namespace QmsStrings;
    for (const auto &mc : this->m_qmsGameState->m_mineCoordinates) {
        if (this->m_qmsGameState->m_mineSweeperButtons.find(mc) != this->m_qmsGameState->m_mineSweeperButtons.end()) {
            this->m_qmsGameState->m_mineSweeperButtons.at(mc)->setHasMine(true);
        } else {
            throw std::runtime_error(GENERIC_ERROR_MESSAGE);
        }
    }
}

 void GameController::determineNeighborMineCounts()
{
    for(std::pair<MineCoordinates, std::shared_ptr<QmsButton> > msbp : this->m_qmsGameState->m_mineSweeperButtons) {
        for (int columnI = msbp.second->columnIndex() - 1; columnI <= msbp.second->columnIndex() + 1; columnI++) {
            for (int rowI = msbp.second->rowIndex() - 1; rowI <= msbp.second->rowIndex() + 1; rowI++) {
                if ((columnI == msbp.second->columnIndex()) && (rowI == msbp.second->rowIndex())) {
                    continue;
                } else if (mineInBounds(MineCoordinates{columnI, rowI})) {
                    MineCoordinates minePairCheck{ columnI, rowI };
                    if (this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck)->hasMine()) {
                        msbp.second->setNumberOfSurroundingMines(msbp.second->numberOfSurroundingMines() + 1);
                    }
                }
            }
        }
    }
}

bool GameController::mineInBounds(const MineCoordinates &coordinatesToCheck) const
{
    int tempColumnIndex = coordinatesToCheck.X();
    int tempRowIndex = coordinatesToCheck.Y();
    return ((tempColumnIndex >= 0) &&
            (tempColumnIndex < this->m_qmsGameState->m_numberOfColumns) &&
            (tempRowIndex >= 0) &&
            (tempRowIndex < this->m_qmsGameState->m_numberOfRows));
}

bool GameController::mineInBounds(int columnIndex, int rowIndex) const
{
    return mineInBounds(MineCoordinates{columnIndex, rowIndex});
}


 void GameController::checkForOtherEmptyMines(QmsButton *msbp)
 {
    for (int columnI = msbp->columnIndex() - 1; columnI <= msbp->columnIndex() + 1; columnI++) {
        for (int rowI = msbp->rowIndex() - 1; rowI <= msbp->rowIndex() + 1; rowI++) {
            if ((columnI == msbp->columnIndex()) && (rowI == msbp->rowIndex())) {
                continue;
            } else if (mineInBounds(columnI, rowI)) {
                MineCoordinates minePairCheck{ columnI, rowI };
                if ((!this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck)->hasMine()) &&
                    (!this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck)->isChecked()) &&
                    (!this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck)->hasQuestionMark()) &&
                    (!this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck)->hasFlag())) {
                    this->m_mainWindow->displayMine(this->m_qmsGameState->m_mineSweeperButtons.at(minePairCheck).get());
                }
            }
        }
    }
}

void GameController::onMineSweeperButtonLeftClicked(QmsButton *msbp)
{
    Q_UNUSED(msbp);
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonRightClicked(QmsButton *msbp)
{
    Q_UNUSED(msbp);
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonLeftClickReleased(QmsButton *msbp)
{
    using namespace QmsStrings;
    if (this->m_qmsGameState->m_gameOver) {
        return;
    }
    if (this->m_qmsGameState->m_initialClickFlag) {
        this->generateRandomMinePlacement(msbp);
        this->m_qmsGameState->m_initialClickFlag = false;
        this->m_qmsGameState->m_gameState = GameState::GameActive;
        try {
            assignAllMines();
            determineNeighborMineCounts();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
            errorBox->setText(GENERIC_ERROR_MESSAGE);
            std::cout << e.what() << std::endl;
            errorBox->exec();
        }
        this->m_qmsGameState->m_gameState = GameState::GameActive;
        emit(gameStarted());
    }    
    if ((msbp->hasFlag()) || (msbp->hasQuestionMark())) {
        msbp->setChecked(false);
    } else if (msbp->hasMine()) {
        emit(mineExplosionEvent());
    } else if (msbp->isChecked() || msbp->isRevealed()) {

    } else {
        incrementNumberOfMovesMade();
        this->m_mainWindow->displayMine(msbp);
        if (msbp->numberOfSurroundingMines() == 0) {
            startResetIconTimer(this->s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT, this->m_mainWindow->gameIcons()->FACE_ICON_BIG_SMILEY);
        } else {
            startResetIconTimer(this->s_DEFAULT_WINKY_FACE_TIMEOUT, this->m_mainWindow->gameIcons()->FACE_ICON_WINKY);
        }
    }
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonRightClickReleased(QmsButton *msbp)
{
    using namespace QmsUtilities;
    using namespace QmsStrings;
    if (this->m_qmsGameState->m_gameOver) {
        return;
    }
    if (this->m_qmsGameState->m_initialClickFlag) {
        generateRandomMinePlacement(msbp);
        try {
            this->assignAllMines();
            this->determineNeighborMineCounts();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> errorBox{new QMessageBox{}};
            errorBox->setText(GENERIC_ERROR_MESSAGE);
            errorBox->setWindowIcon(this->m_mainWindow->gameIcons()->MINE_ICON_48);
            errorBox->exec();
            logString(e.what());
            exit(EXIT_FAILURE);
        }
        this->m_qmsGameState->m_initialClickFlag = false;
        this->m_qmsGameState->m_gameState = GameState::GameActive;
        emit(gameStarted());
    }
    if (msbp->isChecked() || msbp->isRevealed()) {
        //
    } else if (msbp->hasFlag()) {
        msbp->setHasFlag(false);
        msbp->setHasQuestionMark(true);
        msbp->setIcon(this->m_mainWindow->gameIcons()->STATUS_ICON_QUESTION);
        incrementUserMineCountDisplay();
    } else if (msbp->hasQuestionMark()) {
        msbp->setHasQuestionMark(false);
        msbp->setIcon(this->m_mainWindow->gameIcons()->COUNT_MINES_0);
    } else {
        msbp->setHasFlag(true);
        msbp->setIcon(this->m_mainWindow->gameIcons()->STATUS_ICON_FLAG);
        decrementUserMineCountDisplay();
    }
    startResetIconTimer(this->s_DEFAULT_CRAZY_FACE_TIMEOUT, this->m_mainWindow->gameIcons()->FACE_ICON_CRAZY);
    emit(userIsNoLongerIdle());
}


void GameController::onMineSweeperButtonLongLeftClickReleased(QmsButton *msbp)
{
    return this->onMineSweeperButtonRightClickReleased(msbp);
}

void GameController::onMineSweeperButtonLongRightClickReleased(QmsButton *msbp)
{
    return this->onMineSweeperButtonRightClickReleased(msbp);
}

void GameController::onMineDisplayed()
{
    if ((--this->m_qmsGameState->m_unopenedMineCount) == this->m_qmsGameState->m_numberOfMines) {
        emit(winEvent());
    }
}

void GameController::onGameWon()
{
    this->m_qmsGameState->m_gameState = GameState::GameInactive;
}

void GameController::onContextMenuActive()
{
    if (this->m_qmsGameState->m_gameState == GameState::GameActive) {
        this->m_qmsGameState->m_gameState = GameState::GamePaused;
        emit(gamePaused());
        this->gamePaused();
    }
}

void GameController::onContextMenuInactive()
{
    if ((this->m_qmsGameState->m_gameState == GameState::GamePaused) && (!this->m_mainWindow->boardResizeDialogVisible())) {
        this->m_qmsGameState->m_gameState = GameState::GameActive;
        emit(gameResumed());
    }
}

GameState GameController::gameState() const
{
    return this->m_qmsGameState->m_gameState;
}

void GameController::bindMainWindow(std::shared_ptr<MainWindow> mainWindow)
{
    this->m_mainWindow.reset();
    this->m_mainWindow = mainWindow;
}

int GameController::numberOfMovesMade() const
{
    return this->m_qmsGameState->m_numberOfMovesMade;
}

void GameController::incrementNumberOfMovesMade()
{
    emit(numberOfMovesMadeChanged(++this->m_qmsGameState->m_numberOfMovesMade));
}

void GameController::decrementNumberOfMovesMade()
{
    emit(numberOfMovesMadeChanged(--this->m_qmsGameState->m_numberOfMovesMade));
}

void GameController::incrementUserMineCountDisplay()
{
    emit(numberOfMinesRemainingChanged(++this->m_qmsGameState->m_userDisplayNumberOfMines));
}

void GameController::decrementUserMineCountDisplay()
{
    emit(numberOfMinesRemainingChanged(--this->m_qmsGameState->m_userDisplayNumberOfMines));
}

double GameController::DEFAULT_NUMBER_OF_MINES()
{
    return GameController::s_DEFAULT_NUMBER_OF_MINES;
}

int GameController::GAME_TIMER_INTERVAL()
{
    return GameController::s_GAME_TIMER_INTERVAL;
}

int GameController::NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES()
{
    return GameController::s_NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
}

int GameController::EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES()
{
    return GameController::s_EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
}

int GameController::CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES()
{
    return GameController::s_CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES;
}

int GameController::DEFAULT_WINKY_FACE_TIMEOUT()
{
    return GameController::s_DEFAULT_WINKY_FACE_TIMEOUT;
}

int GameController::DEFAULT_CRAZY_FACE_TIMEOUT()
{
    return GameController::s_DEFAULT_CRAZY_FACE_TIMEOUT;
}

int GameController::DEFAULT_BIG_SMILEY_FACE_TIMEOUT()
{
    return GameController::s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT;
}

int GameController::DEFAULT_SLEEPY_FACE_TIMEOUT()
{
    return GameController::s_DEFAULT_SLEEPY_FACE_TIMEOUT;
}

int GameController::LONG_CLICK_THRESHOLD()
{
    return GameController::s_LONG_CLICK_THRESHOLD;
}

int GameController::MILLISECOND_DELAY_DIGITS()
{
    return GameController::s_MILLISECOND_DISPLAY_DIGITS;
}

GameController::~GameController()
{

}
