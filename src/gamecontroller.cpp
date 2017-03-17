/***********************************************************************
*    gamecontroller.cpp:                                               *
*    Main controlling logic for the game                               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
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

const double GameController::s_DEFAULT_NUMBER_OF_MINES{81.0};
const int GameController::s_GAME_TIMER_INTERVAL{100};
const std::pair<double, double> GameController::s_CELL_TO_MINE_RATIOS{std::make_pair(0.15625, 0.15625)};
const int GameController::s_CELL_TO_MINE_THRESHOLD{82};
const int GameController::s_NORMAL_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{8};
const int GameController::s_EDGE_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{5};
const int GameController::s_CORNER_MINE_MAX_NUMBER_OF_NEIGHBOR_MINES{3};
const int GameController::s_DEFAULT_WINKY_FACE_TIMEOUT{250};
const int GameController::s_DEFAULT_CRAZY_FACE_TIMEOUT{250};
const int GameController::s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT{500};
const int GameController::s_DEFAULT_SLEEPY_FACE_TIMEOUT{15000};
const int GameController::s_LONG_CLICK_THRESHOLD{250};
const int GameController::s_MILLISECOND_DISPLAY_DIGITS{1};

GameController::GameController() :
    m_numberOfMines{0},
    m_userDisplayNumberOfMines{0},
    m_initialClickFlag{true},
    m_numberOfColumns{9},
    m_numberOfRows{9},
    m_numberOfMovesMade{0},
    m_gameState{GameState::GAME_INACTIVE},
    m_gameOver{false},
    m_mwPtr{nullptr},
    m_totalButtonCount{this->m_numberOfColumns*this->m_numberOfRows},
    m_unopenedMineCount{this->m_numberOfColumns*this->m_numberOfRows}
{
    using namespace QMineSweeperUtilities;
    this->m_numberOfMines = ((this->m_numberOfColumns * this->m_numberOfRows) < this->s_CELL_TO_MINE_THRESHOLD) ?
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.first)) :
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.second));
    this->m_userDisplayNumberOfMines = this->m_numberOfMines;
}

void GameController::onMineSweeperButtonCreated(std::shared_ptr<QMineSweeperButton> msb)
{
    connect(msb.get(), SIGNAL(leftClicked(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonLeftClicked(std::shared_ptr<QMineSweeperButton>)));
    connect(msb.get(), SIGNAL(rightClicked(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonRightClicked(std::shared_ptr<QMineSweeperButton>)));
    connect(msb.get(), SIGNAL(leftClickReleased(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonLeftClickReleased(std::shared_ptr<QMineSweeperButton>)));
    connect(msb.get(), SIGNAL(rightClickReleased(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonRightClickReleased(std::shared_ptr<QMineSweeperButton>)));
    connect(msb.get(), SIGNAL(longLeftClickReleased(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonLongLeftClickReleased(std::shared_ptr<QMineSweeperButton>)));
    connect(msb.get(), SIGNAL(longRightClickReleased(std::shared_ptr<QMineSweeperButton>)), this, SLOT(onMineSweeperButtonLongLeftClickReleased(std::shared_ptr<QMineSweeperButton>)));
}


int GameController::totalButtonCount() const
{
    return this->m_totalButtonCount;
}

int GameController::unopenedMineCount() const
{
    return this->m_unopenedMineCount;
}

void GameController::onBoardResizeTriggered(int columns, int rows)
{
    using namespace QMineSweeperUtilities;
    this->m_numberOfColumns = columns;
    this->m_numberOfRows = rows;
    this->m_numberOfMines = ((this->m_numberOfColumns * this->m_numberOfRows) < this->s_CELL_TO_MINE_THRESHOLD) ?
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.first)) :
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.second));
    this->m_userDisplayNumberOfMines = this->m_numberOfMines;
    this->m_gameState = GameState::GAME_INACTIVE;
    this->m_mineCoordinates.clear();
    this->m_mineSweeperButtons.clear();
    this->m_initialClickFlag = true;
    this->m_totalButtonCount = this->m_numberOfColumns * this->m_numberOfRows;
    this->m_unopenedMineCount = this->m_totalButtonCount;
    emit(readyToBeginNewGame());
}

void GameController::onGamePaused()
{
    if (!this->m_initialClickFlag) {
        this->m_gameState = GameState::GAME_PAUSED;
    }
}

void GameController::onGameResumed()
{
    if (!this->m_initialClickFlag) {
        this->m_gameState = GameState::GAME_ACTIVE;
    }
}

int GameController::userDisplayNumberOfMines() const
{
    return this->m_userDisplayNumberOfMines;
}

void GameController::onMineExplosionEventTriggered()
{
    this->m_gameState = GameState::GAME_INACTIVE;
    for (auto &it : this->m_mineSweeperButtons) {
        it.second->setBlockClicks(true);
    }
}

void GameController::setNumberOfMinesRemaining(int numberOfMinesRemaining)
{
    this->m_userDisplayNumberOfMines = numberOfMinesRemaining;
    emit (numberOfMinesRemainingChanged(this->m_userDisplayNumberOfMines));
}

int GameController::numberOfColumns() const
{
    return this->m_numberOfColumns;
}

int GameController::numberOfMines() const
{
    return this->m_numberOfMines;
}

void GameController::setNumberOfColumns(int numberOfColumns)
{
    this->m_numberOfColumns = numberOfColumns;
}

int GameController::numberOfRows() const
{
    return this->m_numberOfRows;
}

void GameController::setNumberOfRows(int numberOfRows)
{
    this->m_numberOfRows = numberOfRows;
}

bool GameController::initialClickFlag() const
{
    return this->m_initialClickFlag;
}

void GameController::setInitialClickFlag(bool initialClickFlag)
{
    this->m_initialClickFlag = initialClickFlag;
}

void GameController::addMineSweeperButton(int columnIndex, int rowIndex)
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    try {
        this->m_mineSweeperButtons.emplace(std::make_pair(MineCoordinates(columnIndex, rowIndex), std::make_shared<QMineSweeperButton> (new QMineSweeperButton{columnIndex, rowIndex, nullptr})));
    } catch (std::exception &e) {
        std::cout << STANDARD_EXCEPTION_CAUGHT_IN_ADD_MINESWEEPER_BUTTON_STRING << e.what() << std::endl;
    }
}

std::shared_ptr<QMineSweeperButton> GameController::mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const
{
    using namespace QMineSweeperStrings;
    if (mineInBounds(coordinates) && (m_mineSweeperButtons.find(coordinates) != m_mineSweeperButtons.end())) {
        return this->m_mineSweeperButtons.at(coordinates);
    } else {
        throw std::runtime_error(GENERIC_ERROR_MESSAGE);
    }
}

std::shared_ptr<QMineSweeperButton> GameController::mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const
{
    return this->mineSweeperButtonAtIndex(MineCoordinates(columnIndex, rowIndex));
}

void GameController::startResetIconTimer(unsigned int howLong, const QIcon &icon) const
{
    this->m_mwPtr->setResetButtonIcon(icon);
    QTimer::singleShot(howLong, this->m_mwPtr.get(), SLOT(resetResetButtonIcon()));
}


bool GameController::isCornerButton(std::shared_ptr<QMineSweeperButton> msb) const
{
    return (((msb->columnIndex() == 0) && (msb->rowIndex() == 0)) ||
            ((msb->columnIndex() == 0) && (msb->rowIndex() == this->m_numberOfRows - 1)) ||
            ((msb->columnIndex() == this->m_numberOfColumns - 1) && (msb->rowIndex() == 0)) ||
            ((msb->columnIndex() == this->m_numberOfColumns - 1) && (msb->rowIndex() == this->m_numberOfRows - 1)));
}

bool GameController::isEdgeButton(std::shared_ptr<QMineSweeperButton> msb) const
{
    return ((msb->columnIndex() == 0) ||
            (msb->columnIndex() == this->m_numberOfColumns - 1) ||
            (msb->rowIndex() == 0) ||
            (msb->rowIndex() == this->m_numberOfRows - 1));
}


void GameController::generateRandomMinePlacement(std::shared_ptr<QMineSweeperButton> msb)
{
    using namespace QMineSweeperUtilities;
    MineCoordinates potentialMineCoordinates{0,0};
    while (this->m_mineCoordinates.size() < static_cast<unsigned int>(this->m_numberOfMines)) {
        potentialMineCoordinates = MineCoordinates{randomBetween(0, this->m_numberOfColumns), randomBetween(0, this->m_numberOfRows)};
        if (potentialMineCoordinates == *(msb->mineCoordinates().get())) {
            continue;
        } else {
            this->m_mineCoordinates.emplace(std::make_pair(potentialMineCoordinates.X(), potentialMineCoordinates.Y()));
        }
    }
}

void GameController::clearRandomMinePlacement()
{
    m_mineCoordinates.clear();
}

void GameController::onGameReset()
 {
    for (std::pair<const MineCoordinates, std::shared_ptr<QMineSweeperButton>> msbp : this->m_mineSweeperButtons) {
        msbp.second->setHasFlag(false);
        msbp.second->setHasQuestionMark(false);
        msbp.second->setHasMine(false);
        msbp.second->setIsRevealed(false);
        msbp.second->setNumberOfSurroundingMines(0);
        msbp.second->setBlockClicks(false);
    }
    clearRandomMinePlacement();
    this->m_initialClickFlag = true;
    this->m_gameOver = false;
    this->m_userDisplayNumberOfMines = this->m_numberOfMines;
    this->m_gameState = GameState::GAME_INACTIVE;
    this->m_numberOfMovesMade = 0;
    this->m_unopenedMineCount = this->m_numberOfColumns*this->m_numberOfRows;
}

void GameController::setGameOver(bool gameOver)
{
    this->m_gameState = GameState::GAME_INACTIVE;
    this->m_gameOver = gameOver;
}

bool GameController::gameOver() const
{
    return this->m_gameOver;
}


bool GameController::coordinatePairExists(const MineCoordinates &coordinatesToCheck) const
{
    for (const std::pair<int, int> &mc : this->m_mineCoordinates) {
        if (coordinatesToCheck == mc) {
            return true;
        }
    }
    return false;
}

void GameController::setNumberOfMovesMade(int numberOfMovesMade)
{
    this->m_numberOfMovesMade = numberOfMovesMade;
}


void GameController::assignAllMines()
{
    using namespace QMineSweeperStrings;
    for (const std::pair<int, int> &mc : this->m_mineCoordinates) {
        if (this->m_mineSweeperButtons.find(mc) != this->m_mineSweeperButtons.end()) {
            this->m_mineSweeperButtons.at(mc)->setHasMine(true);
        } else {
            throw std::runtime_error(GENERIC_ERROR_MESSAGE);
        }
    }
}

 void GameController::determineNeighborMineCounts()
{
    for(std::pair<MineCoordinates, std::shared_ptr<QMineSweeperButton> > msbp : this->m_mineSweeperButtons) {
        for (int columnI = msbp.second->columnIndex() - 1; columnI <= msbp.second->columnIndex() + 1; columnI++) {
            for (int rowI = msbp.second->rowIndex() - 1; rowI <= msbp.second->rowIndex() + 1; rowI++) {
                if ((columnI == msbp.second->columnIndex()) && (rowI == msbp.second->rowIndex())) {
                    continue;
                } else if (mineInBounds(MineCoordinates{columnI, rowI})) {
                    MineCoordinates minePairCheck{ columnI, rowI };
                    if (m_mineSweeperButtons.at(minePairCheck)->hasMine()) {
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
            (tempColumnIndex < m_numberOfColumns) &&
            (tempRowIndex >= 0) &&
            (tempRowIndex < m_numberOfRows));
}

bool GameController::mineInBounds(int columnIndex, int rowIndex) const
{
    return mineInBounds(MineCoordinates{columnIndex, rowIndex});
}


 void GameController::checkForOtherEmptyMines(std::shared_ptr<QMineSweeperButton> msb)
 {
    for (int columnI = msb->columnIndex() - 1; columnI <= msb->columnIndex() + 1; columnI++) {
        for (int rowI = msb->rowIndex() - 1; rowI <= msb->rowIndex() + 1; rowI++) {
            if ((columnI == msb->columnIndex()) && (rowI == msb->rowIndex())) {
                continue;
            } else if (mineInBounds(columnI, rowI)) {
                MineCoordinates minePairCheck{ columnI, rowI };
                if ((!this->m_mineSweeperButtons.at(minePairCheck)->hasMine()) &&
                    (!this->m_mineSweeperButtons.at(minePairCheck)->isChecked()) &&
                    (!this->m_mineSweeperButtons.at(minePairCheck)->hasQuestionMark()) &&
                    (!this->m_mineSweeperButtons.at(minePairCheck)->hasFlag())) {
                    this->m_mwPtr->displayMine(this->m_mineSweeperButtons.at(minePairCheck));
                }
            }
        }
    }
}

void GameController::onMineSweeperButtonLeftClicked(std::shared_ptr<QMineSweeperButton> msb)
{
    Q_UNUSED(msb);
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonRightClicked(std::shared_ptr<QMineSweeperButton> msb)
{
    Q_UNUSED(msb);
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonLeftClickReleased(std::shared_ptr<QMineSweeperButton> msb)
{
    using namespace QMineSweeperStrings;
    if (this->m_gameOver) {
        return;
    }
    if (this->m_initialClickFlag) {
        this->generateRandomMinePlacement(std::make_shared<QMineSweeperButton>(msb));
        this->m_initialClickFlag = false;
        this->m_gameState = GameState::GAME_ACTIVE;
        try {
            assignAllMines();
            determineNeighborMineCounts();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            errorBox->setText(GENERIC_ERROR_MESSAGE);
            std::cout << e.what() << std::endl;
            errorBox->exec();
        }
        this->m_gameState = GameState::GAME_ACTIVE;
        emit(gameStarted());
    }    
    if ((msb->hasFlag()) || (msb->hasQuestionMark())) {
        msb->setChecked(false);
    } else if (msb->hasMine()) {
        emit(mineExplosionEvent());
    } else if (msb->isChecked() || msb->isRevealed()) {

    } else {
        incrementNumberOfMovesMade();
        this->m_mwPtr->displayMine(msb);
        if (msb->numberOfSurroundingMines() == 0) {
            startResetIconTimer(this->s_DEFAULT_BIG_SMILEY_FACE_TIMEOUT, this->m_mwPtr->qmsiPtr()->FACE_ICON_BIG_SMILEY);
        } else {
            startResetIconTimer(this->s_DEFAULT_WINKY_FACE_TIMEOUT, this->m_mwPtr->qmsiPtr()->FACE_ICON_WINKY);
        }
    }
    emit(userIsNoLongerIdle());
}

void GameController::onMineSweeperButtonRightClickReleased(std::shared_ptr<QMineSweeperButton> msb)
{
    using namespace QMineSweeperUtilities;
    using namespace QMineSweeperStrings;
    if (this->m_gameOver) {
        return;
    }
    if (this->m_initialClickFlag) {
        generateRandomMinePlacement(std::make_shared<QMineSweeperButton>(msb));
        try {
            this->assignAllMines();
            this->determineNeighborMineCounts();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            errorBox->setText(GENERIC_ERROR_MESSAGE);
            errorBox->setWindowIcon(this->m_mwPtr->qmsiPtr()->MINE_ICON_48);
            errorBox->exec();
            logString(e.what());
            exit(EXIT_FAILURE);
        }
        this->m_initialClickFlag = false;
        this->m_gameState = GameState::GAME_ACTIVE;
        emit(gameStarted());
    }
    if (msb->isChecked() || msb->isRevealed()) {
        //
    } else if (msb->hasFlag()) {
        msb->setHasFlag(false);
        msb->setHasQuestionMark(true);
        msb->setIcon(this->m_mwPtr->qmsiPtr()->STATUS_ICON_QUESTION);
        incrementUserMineCountDisplay();
    } else if (msb->hasQuestionMark()) {
        msb->setHasQuestionMark(false);
        msb->setIcon(this->m_mwPtr->qmsiPtr()->COUNT_MINES_0);
    } else {
        msb->setHasFlag(true);
        msb->setIcon(this->m_mwPtr->qmsiPtr()->STATUS_ICON_FLAG);
        decrementUserMineCountDisplay();
    }
    startResetIconTimer(this->s_DEFAULT_CRAZY_FACE_TIMEOUT, this->m_mwPtr->qmsiPtr()->FACE_ICON_CRAZY);
    emit(userIsNoLongerIdle());
}


void GameController::onMineSweeperButtonLongLeftClickReleased(std::shared_ptr<QMineSweeperButton> msb)
{
    return this->onMineSweeperButtonRightClickReleased(msb);
}

void GameController::onMineSweeperButtonLongRightClickReleased(std::shared_ptr<QMineSweeperButton> msb)
{
    return this->onMineSweeperButtonRightClickReleased(msb);
}

void GameController::onMineDisplayed()
{
    if ((--this->m_unopenedMineCount) == this->m_numberOfMines) {
        emit(winEvent());
    }
}

void GameController::onGameWon()
{
    this->m_gameState = GameState::GAME_INACTIVE;
}

void GameController::onContextMenuActive()
{
    if (this->m_gameState == GameState::GAME_ACTIVE) {
        this->m_gameState = GameState::GAME_PAUSED;
        emit(gamePaused());
    }
}

void GameController::onContextMenuInactive()
{
    if ((this->m_gameState == GameState::GAME_PAUSED) && (!this->m_mwPtr->boardResizeDialogVisible())) {
        this->m_gameState = GameState::GAME_ACTIVE;
        emit(gameResumed());
    }
}

GameState GameController::gameState() const
{
    return this->m_gameState;
}

void GameController::bindMainWindow(std::shared_ptr<MainWindow> mw)
{
    this->m_mwPtr.reset();
    this->m_mwPtr = mw;
}

int GameController::numberOfMovesMade() const
{
    return this->m_numberOfMovesMade;
}

void GameController::incrementNumberOfMovesMade()
{
    emit(numberOfMovesMadeChanged(++this->m_numberOfMovesMade));
}

void GameController::decrementNumberOfMovesMade()
{
    emit(numberOfMovesMadeChanged(--this->m_numberOfMovesMade));
}

void GameController::incrementUserMineCountDisplay()
{
    emit(numberOfMinesRemainingChanged(++this->m_userDisplayNumberOfMines));
}

void GameController::decrementUserMineCountDisplay()
{
    emit(numberOfMinesRemainingChanged(--this->m_userDisplayNumberOfMines));
}

double GameController::DEFAULT_NUMBER_OF_MINES()
{
    return GameController::s_DEFAULT_NUMBER_OF_MINES;
}

int GameController::GAME_TIMER_INTERVAL()
{
    return GameController::s_GAME_TIMER_INTERVAL;
}

std::pair<double, double> GameController::CELL_TO_MINE_RATIOS()
{
    return GameController::s_CELL_TO_MINE_RATIOS;
}

int GameController::CELL_TO_MINE_THRESHOLD()
{
    return GameController::s_CELL_TO_MINE_THRESHOLD;
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
