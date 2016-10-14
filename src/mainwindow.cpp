/***********************************************************************
*    mainwindow.cpp:                                                   *
*    Main window for displaying the game                               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source  file for QMineSweeper:                          *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation of a custom QMainWindow class  *
*    MainWindow is the default container for a Qt Desktop Application  *
*    The source code is released under the GNU LGPL                    *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_board-size-dialog.h"

#if defined(__ANDROID__)
    const int MainWindow::s_TASKBAR_HEIGHT{10};
    const int MainWindow::s_HEIGHT_SCALE_FACTOR{18};
    const int MainWindow::s_WIDTH_SCALE_FACTOR{28};
    const int MainWindow::s_NUMBER_OF_HORIZONTAL_MARGINS{2};
    const int MainWindow::s_NUMBER_OF_VERTIAL_MARGINS{4};
    const int MainWindow::s_DEFAULT_MINE_SIZE_SCALE_FACTOR{1};
    const double MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR{0.8};
#else
    const int MainWindow::s_TASKBAR_HEIGHT{10};
    const int MainWindow::s_HEIGHT_SCALE_FACTOR{18};
    const int MainWindow::s_WIDTH_SCALE_FACTOR{28};
    const int MainWindow::s_NUMBER_OF_HORIZONTAL_MARGINS{2};
    const int MainWindow::s_NUMBER_OF_VERTIAL_MARGINS{4};
    const int MainWindow::s_DEFAULT_MINE_SIZE_SCALE_FACTOR{19};
    const double MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR{0.75};
#endif

MainWindow::MainWindow(std::shared_ptr<QMineSweeperIcons> qmsiPtr,
                       std::shared_ptr<QMineSweeperSoundEffects> qmssePtr,
                       std::shared_ptr<QMineSweeperStrings> qmsstrPtr,
                       std::shared_ptr<GameController> gcPtr,
                       std::shared_ptr<QDesktopWidget> qdwPtr,
                       QWidget *parent) :
    QMainWindow{parent},
#if defined(__ANDROID__)
    m_eventTimer{new QTimer()},
    m_playTimer{new GameTimer()},
    m_userIdleTimer{new GameTimer()},
    m_bsui{new Ui::Dialog()},
    m_ui{new Ui::MainWindow()},
    m_boardSizeDialog{new CustomDialog()},
#else
    m_eventTimer{std::make_unique<QTimer>()},
    m_playTimer{std::make_unique<EventTimer>()},
    m_userIdleTimer{std::make_unique<EventTimer>()},
    m_bsui{std::make_unique<Ui::Dialog>()},
    m_ui{std::make_unique<Ui::MainWindow>()},
    m_boardSizeDialog{std::make_unique<CustomDialog>()},
#endif
    m_qmsiPtr{qmsiPtr},
    m_qmssePtr{qmssePtr},
    m_qmsstrPtr{qmsstrPtr},
    m_gcPtr{gcPtr},
    m_qdwPtr{qdwPtr},
    m_xPlacement{0},
    m_yPlacement{0},
    m_currentDefaultMineSize{QSize{0,0}},
    m_currentMaxMineSize{QSize{0,0}},
    m_maxMineSizeCacheIsValid{false},
    m_tempPauseFlag{false}
{
    using namespace QMineSweeperUtilities;
    this->m_ui->setupUi(this);
    this->m_bsui->setupUi(this->m_boardSizeDialog.get());
    this->m_boardSizeDialog->setWindowTitle(this->m_qmsstrPtr->MAIN_WINDOW_TITLE);
    this->m_boardSizeDialog->setWindowIcon(this->m_qmsiPtr->MINE_ICON_72);
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    this->m_eventTimer->setInterval(this->m_gcPtr->GAME_TIMER_INTERVAL());
    this->m_bsui->columnsBox->setTabOrder(this->m_bsui->columnsBox, this->m_bsui->rowsBox);

    connect(this->m_ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(this->m_ui->actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(onAboutQtActionTriggered()));
    connect(this->m_ui->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(this->m_ui->resetButton, SIGNAL(clicked(bool)), this, SLOT(onResetButtonClicked()));
    connect(this->m_ui->actionBoardSize, SIGNAL(triggered(bool)), this, SLOT(onChangeBoardSizeActionTriggered()));
    connect(this->m_bsui->okayButton, SIGNAL(clicked(bool)), this, SLOT(onBsuiOkayButtonClicked()));
    connect(this->m_bsui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onBsuiCancelButtonClicked()));
    connect(this, SIGNAL(boardResize(int, int)), this->m_gcPtr.get(), SLOT(onBoardResizeTriggered(int, int)));
    connect(this->m_gcPtr.get(), SIGNAL(readyToBeginNewGame()), this, SLOT(setupNewGame()));
    connect(this, SIGNAL(mineDisplayed()), this->m_gcPtr.get(), SLOT(onMineDisplayed()));
    connect(this->m_gcPtr.get(), SIGNAL(winEvent()), this, SLOT(onGameWon()));
    connect(this, SIGNAL(winEvent()), this->m_gcPtr.get(), SLOT(onGameWon()));
    connect(this, SIGNAL(gamePaused()), this->m_gcPtr.get(), SLOT(onGamePaused()));
    connect(this->m_gcPtr.get(), SIGNAL(userIsNoLongerIdle()), this, SLOT(startUserIdleTimer()));

    connect(this->m_eventTimer.get(), SIGNAL(timeout()), this, SLOT(eventLoop()));
    connect(this->m_gcPtr.get(), SIGNAL(gameStarted()), this, SLOT(onGameStarted()));
    connect(this, SIGNAL(resetGame()), this->m_gcPtr.get(), SLOT(onGameReset()));

    connect(this->m_ui->menuFile, SIGNAL(aboutToShow()), this->m_gcPtr.get(), SLOT(onContextMenuActive()));
    connect(this->m_ui->menuPreferences, SIGNAL(aboutToShow()), this->m_gcPtr.get(), SLOT(onContextMenuActive()));
    connect(this->m_ui->menuHelp, SIGNAL(aboutToShow()), this->m_gcPtr.get(), SLOT(onContextMenuActive()));

    connect(this->m_ui->menuFile, SIGNAL(aboutToHide()), this->m_gcPtr.get(), SLOT(onContextMenuInactive()));
    connect(this->m_ui->menuPreferences, SIGNAL(aboutToHide()), this->m_gcPtr.get(), SLOT(onContextMenuInactive()));
    connect(this->m_ui->menuHelp, SIGNAL(aboutToHide()), this->m_gcPtr.get(), SLOT(onContextMenuInactive()));

    connect(this->m_gcPtr.get(), SIGNAL(gameResumed()), this, SLOT(onGameResumed()));
    connect(this, SIGNAL(gameResumed()), this->m_gcPtr.get(), SLOT(onGameResumed()));
    connect(this->m_gcPtr.get(), SIGNAL(gamePaused()), this->m_gcPtr.get(), SLOT(onGamePaused()));

    connect(this, SIGNAL(mineExplosionEvent()), this->m_gcPtr.get(), SLOT(onMineExplosionEventTriggered()));
    connect(this->m_gcPtr.get(), SIGNAL(mineExplosionEvent()), this, SLOT(onMineExplosionEventTriggered()));
    connect(this->m_boardSizeDialog.get(), SIGNAL(aboutToClose()), this, SLOT(onCustomDialogClosed()));

    #if defined(__ANDROID__)
        std::unique_ptr<QRect> avail{new QRect(this->m_qDesktopWidget->availableGeometry())};
        this->m_reductionSizeScaleFactor = 1;
    #else
        std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qdwPtr->availableGeometry())};
        if (avail->height() > 800) {
            this->m_reductionSizeScaleFactor = 16.0;
        } else {
            this->m_reductionSizeScaleFactor = 14.0;
        }
    #endif
    this->m_eventTimer->start();
}

bool MainWindow::boardResizeDialogVisible()
{
    return this->m_boardSizeDialog->isVisible();
}

void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if ((!this->isHidden()) && (!this->isMinimized())) {
        if (!this->m_gcPtr->initialClickFlag() && !this->m_gcPtr->gameOver()) {
            emit (gameResumed());
        }
    }
}

void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    if ((this->isHidden()) || (this->isMinimized())) {
        emit (gamePaused());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit(gamePaused());
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this, this->m_qmsstrPtr->CLOSE_APPLICATION_WINDOW_TITLE, this->m_qmsstrPtr->CLOSE_APPLICATION_WINDOW_PROMPT, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        this->m_boardSizeDialog->close();
        event->accept();
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
        if (!this->m_gcPtr->initialClickFlag() && !this->m_gcPtr->gameOver()) {
            emit (gameResumed());
        }
    }
}

void MainWindow::onCustomDialogClosed()
{
    this->setEnabled(true);
    if (!this->m_gcPtr->initialClickFlag() && !this->m_gcPtr->gameOver()) {
        emit (gameResumed());
    }
}

void MainWindow::onGameWon()
{
    using namespace QMineSweeperUtilities;
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_BIG_SMILEY);
    this->m_gcPtr->setGameOver(true);

    for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex < this->m_gcPtr->numberOfColumns(); columnIndex++) {
            std::shared_ptr<MineSweeperButton> tempMsb = this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex);
            if(tempMsb->hasMine()) {
                if (tempMsb->hasFlag()) {
                    tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_CHECK);
                    tempMsb->setChecked(true);
                } else {
                    tempMsb->setIcon(this->m_qmsiPtr->MINE_ICON_72);
                    tempMsb->setChecked(true);
                    tempMsb->setStyleSheet(this->m_qmsstrPtr->UNCOVERED_MINE_STYLESHEET);
                }
            } else if (tempMsb->hasFlag()) {
                tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_X);
                tempMsb->setChecked(true);
            }
            tempMsb->setIsRevealed(true);
        }
    }
#if defined(__ANDROID__)
    std::unique_ptr<QMessageBox> winBox{new QMessageBox()};
#else
    std::unique_ptr<QMessageBox> winBox{std::make_unique<QMessageBox>()};
#endif
    winBox->setWindowTitle(this->m_qmsstrPtr->MAIN_WINDOW_TITLE);
    winBox->setText(this->m_qmsstrPtr->WIN_DIALOG_BASE + toQString(this->m_gcPtr->numberOfMovesMade()) + this->m_qmsstrPtr->WIN_DIALOG_MIDDLE + this->statusBar()->currentMessage());
    winBox->setWindowIcon(this->m_qmsiPtr->MINE_ICON_48);
    winBox->exec();
}

void MainWindow::centerAndFitWindow()
{
    this->setFixedSize(this->minimumSize());
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int MainWindow::xPlacement() const
{
    return this->m_xPlacement;
}

int MainWindow::yPlacement() const
{
    return this->m_yPlacement;
}

void MainWindow::calculateXYPlacement()
{
#if defined(__ANDROID__)
    std::unique_ptr<QRect> avail{new QRect(this->m_qDesktopWidget->availableGeometry())};
#else
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qdwPtr->availableGeometry())};
#endif
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - this->s_TASKBAR_HEIGHT;
}


void MainWindow::onGameStarted()
{
    startGameTimer();
    startUserIdleTimer();
}

void MainWindow::onGamePaused()
{
    for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gcPtr->numberOfColumns(); columnIndex++) {
            this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex)->setEnabled(false);
        }
    }
}

void MainWindow::setupNewGame()
{
    QLayoutItem *wItem;
    while ((wItem = this->m_ui->mineFrame->layout()->takeAt(0)) != 0) {
        delete wItem;
    }
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    populateMineField();
    centerAndFitWindow();
}

void MainWindow::onGameResumed()
{
    for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gcPtr->numberOfColumns(); columnIndex++) {
            this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex)->setEnabled(true);
        }
    }
}

void MainWindow::displayMine(std::shared_ptr<MineSweeperButton> msb)
{
    drawNumberOfSurroundingMines(msb);
    msb->setFlat(true);
    msb->setChecked(true);
    emit(mineDisplayed());
    if (msb->numberOfSurroundingMines() == 0) {
        this->m_gcPtr->checkForOtherEmptyMines(msb);
    }
}
void MainWindow::populateMineField()
{
    for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex <this->m_gcPtr->numberOfColumns(); columnIndex++) {
            this->m_gcPtr->addMineSweeperButton(columnIndex, rowIndex);
            std::shared_ptr<MineSweeperButton> tempPtr = this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex);
            this->m_ui->mineFrameGridLayout->addWidget(tempPtr.get(), rowIndex, columnIndex, 1, 1);
            tempPtr->setFixedSize(getMaxMineSize());
            connect(tempPtr.get(), SIGNAL(leftClicked(std::shared_ptr<MineSweeperButton>)), this->m_gcPtr.get(), SLOT(onMineSweeperButtonLeftClicked(std::shared_ptr<MineSweeperButton>)));
            connect(tempPtr.get(), SIGNAL(rightClicked(std::shared_ptr<MineSweeperButton>)), this->m_gcPtr.get(), SLOT(onMineSweeperButtonRightClicked(std::shared_ptr<MineSweeperButton>)));
            connect(tempPtr.get(), SIGNAL(leftClickReleased(std::shared_ptr<MineSweeperButton>)), this->m_gcPtr.get(), SLOT(onMineSweeperButtonLeftClickReleased(std::shared_ptr<MineSweeperButton>)));
            connect(tempPtr.get(), SIGNAL(rightClickReleased(std::shared_ptr<MineSweeperButton>)), this->m_gcPtr.get(), SLOT(onMineSweeperButtonRightClickReleased(std::shared_ptr<MineSweeperButton>)));
            tempPtr->setIconSize(tempPtr->size() * MainWindow::s_MINE_ICON_REDUCTION_SCALE_FACTOR);
            this->m_saveStyleSheet = tempPtr->styleSheet();
            tempPtr->setCheckable(true);
        }
    }
    this->m_saveStyleSheet = this->m_gcPtr->mineSweeperButtonAtIndex(0, 0)->styleSheet();
}

void MainWindow::invalidateSizeCaches()
{
    this->m_maxMineSizeCacheIsValid = false;
    this->m_iconReductionSizeCacheIsValid = false;
}

QSize MainWindow::getIconReductionSize()
{
    using namespace QMineSweeperUtilities;
    if (!this->m_iconReductionSizeCacheIsValid) {
        int reductionSize{roundIntuitively(m_reductionSizeScaleFactor - (this->m_gcPtr->totalButtonCount()/this->m_gcPtr->DEFAULT_NUMBER_OF_MINES()))};
        if (reductionSize <= 0) {
            reductionSize = 0;
        }
        this->m_currentIconReductionSize = QSize{reductionSize, reductionSize};
        this->m_iconReductionSizeCacheIsValid = true;
    }
    return this->m_currentIconReductionSize;
}

QSize MainWindow::getMaxMineSize()
{
    if (!this->m_maxMineSizeCacheIsValid) {
        int defaultMineSize{this->m_qdwPtr->availableGeometry().height()/this->s_DEFAULT_MINE_SIZE_SCALE_FACTOR};
        this->m_currentDefaultMineSize = QSize{defaultMineSize, defaultMineSize};
        int statusBarHeight{this->m_ui->statusBar->height()};
        int menuBarHeight{this->m_ui->menuBar->height()};
        int titleFrameHeight{this->m_ui->titleFrame->height()};
        int gridSpacingHeight{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_VERTIAL_MARGINS};
        int gridSpacingWidth{this->centralWidget()->layout()->margin()*this->s_NUMBER_OF_HORIZONTAL_MARGINS};
        int heightScale{this->m_qdwPtr->availableGeometry().height()/this->s_HEIGHT_SCALE_FACTOR};
        int widthScale{this->m_qdwPtr->availableGeometry().width()/this->s_WIDTH_SCALE_FACTOR};
        int extraHeight{statusBarHeight + menuBarHeight + titleFrameHeight + gridSpacingHeight};
        int x{(this->m_qdwPtr->availableGeometry().height()-extraHeight-s_TASKBAR_HEIGHT-heightScale)/this->m_gcPtr->numberOfRows()};
        int y{(this->m_qdwPtr->availableGeometry().width()-gridSpacingWidth-widthScale)/this->m_gcPtr->numberOfColumns()};
        if ((x < 5) || (y < 5)) {
            //TODO: Deal with too small to play mines
            #if defined(__ANDROID__)
                std::unique_ptr<QMessageBox> errorBox{new QMessageBox()};
            #else
                std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            #endif
            errorBox->setWindowTitle(this->m_qmsstrPtr->MAIN_WINDOW_TITLE);
            errorBox->setText(this->m_qmsstrPtr->GENERIC_ERROR_MESSAGE);
            errorBox->setWindowIcon(this->m_qmsiPtr->MINE_ICON_48);
            errorBox->exec();
            exit(EXIT_FAILURE);
            QCoreApplication::exit(EXIT_FAILURE);
        }
        if ((x >= m_currentDefaultMineSize.width()) && (y >= m_currentDefaultMineSize.height())) {
            this->m_currentMaxMineSize = this->m_currentDefaultMineSize;
        } else if (x < m_currentDefaultMineSize.width()) {
            if (y < x) {
                this->m_currentMaxMineSize = QSize{y, y};
            } else {
                this->m_currentMaxMineSize = QSize{x, x};
            }
        } else {
            this->m_currentMaxMineSize = QSize{y, y};
        }
        this->m_maxMineSizeCacheIsValid = true;
    }
    return this->m_currentMaxMineSize;
}

void MainWindow::resizeResetIcon()
{
#if defined(__ANDROID__)
    this->m_ui->resetButton->setFixedSize(this->m_gameController->mineSweeperButtonAtIndex(0, 0)->size());
    this->m_ui->numberOfMoves->setFixedSize(this->m_ui->numberOfMoves->size() * 3);
    this->m_ui->minesRemaining->setFixedSize(this->m_ui->numberOfMoves->size());
    this->m_ui->resetButton->setIconSize(this->m_ui->resetButton->size()*4);
#else
    this->m_ui->resetButton->setIconSize(this->m_ui->resetButton->size() - this->getIconReductionSize());
#endif
}

void MainWindow::resetResetButtonIcon()
{
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
}

void MainWindow::setResetButtonIcon(const QIcon &icon)
{
    this->m_ui->resetButton->setIcon(icon);
}

void MainWindow::displayAllMines()
{
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_FROWNY);
    this->m_gcPtr->setGameOver(true);
    for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
        for (int columnIndex = 0; columnIndex < this->m_gcPtr->numberOfColumns(); columnIndex++) {
            std::shared_ptr<MineSweeperButton> tempMsb = this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex);
            if(tempMsb->hasMine()) {
                if (tempMsb->hasFlag()) {
                    tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_CHECK);
                    tempMsb->setChecked(true);
                } else {
                    tempMsb->setIcon(this->m_qmsiPtr->MINE_ICON_72);
                    tempMsb->setChecked(true);
                    tempMsb->setStyleSheet(this->m_qmsstrPtr->UNCOVERED_MINE_STYLESHEET);
                }
            } else if (tempMsb->hasFlag()) {
                tempMsb->setIcon(this->m_qmsiPtr->STATUS_ICON_FLAG_X);
                tempMsb->setChecked(true);
            }
            tempMsb->setIsRevealed(true);
        }
    }
}

QString MainWindow::saveStyleSheet() const
{
    return this->m_saveStyleSheet;
}

std::shared_ptr<QMineSweeperIcons> MainWindow::qmsiPtr() const
{
    return this->m_qmsiPtr;
}

std::shared_ptr<QMineSweeperSoundEffects> MainWindow::qmssePtr() const
{
    return this->m_qmssePtr;
}

std::shared_ptr<QMineSweeperStrings> MainWindow::qmsstrPtr() const
{
    return this->m_qmsstrPtr;
}

void MainWindow::onResetButtonClicked()
{
    emit(gamePaused());
    if (!this->m_gcPtr->gameOver() && !this->m_gcPtr->initialClickFlag()) {
        QMessageBox::StandardButton userReply;
        userReply = QMessageBox::question(this, this->m_qmsstrPtr->START_NEW_GAME_WINDOW_TITLE, this->m_qmsstrPtr->START_NEW_GAME_PROMPT, QMessageBox::Yes|QMessageBox::No);
        if (userReply == QMessageBox::Yes) {
            this->doGameReset();
        } else {
            emit(gameResumed());
        }
    } else {
        this->doGameReset();
    }
}

void MainWindow::doGameReset()
{
    this->m_boardSizeDialog->hide();
    for (int columnIndex = 0; columnIndex < this->m_gcPtr->numberOfColumns(); columnIndex++) {
        for (int rowIndex = 0; rowIndex < this->m_gcPtr->numberOfRows(); rowIndex++) {
            std::shared_ptr<MineSweeperButton> tempMsb{this->m_gcPtr->mineSweeperButtonAtIndex(columnIndex, rowIndex)};
            tempMsb->setChecked(false);
            tempMsb->setFlat(false);
            tempMsb->setStyleSheet(this->m_saveStyleSheet);
            tempMsb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
            tempMsb->setEnabled(true);
            tempMsb->setIsRevealed(false);
        }
    }
    emit(resetGame());
    this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SMILEY);
    this->m_ui->statusBar->showMessage(this->m_qmsstrPtr->START_NEW_GAME_INSTRUCTION);
}

void MainWindow::eventLoop()
{
    updateLCDDisplays();
    updateVisibleGameTimer();
    updateGeometry();
    updateUserIdleTimer();
}

void MainWindow::updateGeometry()
{
    if (this->size() != this->minimumSize()) {
        centerAndFitWindow();
    }
}

void MainWindow::startGameTimer()
{
    this->m_playTimer->start();
}

void MainWindow::startUserIdleTimer()
{
    this->m_userIdleTimer->start();
}

void MainWindow::updateVisibleGameTimer()
{
    using namespace QMineSweeperUtilities;
    QString gameTime{""};
    if (this->m_gcPtr->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_playTimer->isPaused()) {
            this->m_playTimer->unpause();
        }
        this->m_playTimer->update();
        if (this->m_playTimer->hours() == 0) {
            gameTime = toQString(this->m_playTimer->minutes()) + ":" + toQString(this->m_playTimer->seconds()) + "." + toQString(this->m_playTimer->milliseconds());
        } else {
            gameTime = toQString(this->m_playTimer->hours()) + ":" + toQString(this->m_playTimer->minutes()) + ":" + toQString(this->m_playTimer->seconds()) + "." + toQString(this->m_playTimer->milliseconds());
        }
        this->m_ui->statusBar->showMessage(gameTime);
    } else if (!this->m_gcPtr->initialClickFlag()){
        this->m_playTimer->pause();
        this->m_playTimer->update();
        if (this->m_playTimer->hours() == 0) {
            gameTime = toQString(this->m_playTimer->minutes()) + ":" + toQString(this->m_playTimer->seconds()) + "." + toQString(this->m_playTimer->milliseconds());
        } else {
            gameTime = toQString(this->m_playTimer->hours()) + ":" + toQString(this->m_playTimer->minutes()) + ":" + toQString(this->m_playTimer->seconds()) + "." + toQString(this->m_playTimer->milliseconds());
        }
        this->m_ui->statusBar->showMessage(gameTime);
    } else {
        this->m_ui->statusBar->showMessage(this->m_qmsstrPtr->START_NEW_GAME_INSTRUCTION);
    }
}

void MainWindow::updateUserIdleTimer()
{
    if (this->m_gcPtr->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->isPaused()) {
            this->m_userIdleTimer->unpause();
        }
        this->m_playTimer->update();
    } else if (!this->m_gcPtr->initialClickFlag()) {
        this->m_userIdleTimer->pause();
        this->m_userIdleTimer->update();
    }
    if (this->m_gcPtr->gameState() == GameState::GAME_ACTIVE) {
        if (this->m_userIdleTimer->totalTimeMilliseconds() >= this->m_gcPtr->DEFAULT_SLEEPY_FACE_TIMEOUT()) {
            this->m_ui->resetButton->setIcon(this->m_qmsiPtr->FACE_ICON_SLEEPY);
        }
    }
}

void MainWindow::updateLCDDisplays()
{
    using namespace QMineSweeperUtilities;
    if (this->m_gcPtr->userDisplayNumberOfMines() > 999) {
        this->m_ui->minesRemaining->display("999");
    } else if (this->m_gcPtr->userDisplayNumberOfMines() <= 0) {
        this->m_ui->minesRemaining->display("000");
    } else if (this->m_gcPtr->userDisplayNumberOfMines() < 10) {
        this->m_ui->minesRemaining->display("00" + toQString(this->m_gcPtr->userDisplayNumberOfMines()));
    } else if (this->m_gcPtr->userDisplayNumberOfMines() < 100) {
        this->m_ui->minesRemaining->display("0" + toQString(this->m_gcPtr->userDisplayNumberOfMines()));
    } else {
        this->m_ui->minesRemaining->display(this->m_gcPtr->userDisplayNumberOfMines());
    }

    if (this->m_gcPtr->numberOfMovesMade() > 999) {
        this->m_ui->numberOfMoves->display("999");
    } else if (this->m_gcPtr->numberOfMovesMade() <= 0) {
        this->m_ui->numberOfMoves->display("000");
    } else if (this->m_gcPtr->numberOfMovesMade() < 10) {
        this->m_ui->numberOfMoves->display("00" + toQString(this->m_gcPtr->numberOfMovesMade()));
    } else if (this->m_gcPtr->numberOfMovesMade() < 100) {
        this->m_ui->numberOfMoves->display("0" + toQString(this->m_gcPtr->numberOfMovesMade()));
    } else {
        this->m_ui->numberOfMoves->display(this->m_gcPtr->numberOfMovesMade());
    }
}

void MainWindow::onChangeBoardSizeActionTriggered()
{
    using namespace QMineSweeperUtilities;
    this->setEnabled(false);
    this->m_bsui->currentDimensionsLabel->setText(toQString(this->m_gcPtr->numberOfColumns()) + "x" + toQString(this->m_gcPtr->numberOfRows()));
    this->m_bsui->columnsBox->setValue(this->m_gcPtr->numberOfColumns());
    this->m_bsui->rowsBox->setValue(this->m_gcPtr->numberOfRows());
    this->m_boardSizeDialog->show();
    emit(gamePaused());
}

void MainWindow::onBsuiOkayButtonClicked()
{
    using namespace QMineSweeperUtilities;
    this->setEnabled(true);
    this->m_boardSizeDialog->hide();
    int maybeNewColumns{this->m_bsui->columnsBox->value()};
    int maybeNewRows{this->m_bsui->rowsBox->value()};
    QMessageBox::StandardButton userReply;
    QString questionBoxMessage = this->m_qmsstrPtr->RESIZE_BOARD_DIALOG_BASE + toQString(maybeNewColumns) + "x" + toQString(maybeNewRows) + " game?";
    userReply = QMessageBox::question(this, this->m_qmsstrPtr->START_NEW_GAME_WINDOW_TITLE, questionBoxMessage, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        this->invalidateSizeCaches();
        emit(boardResize(maybeNewColumns, maybeNewRows));
    } else {
        if (!this->m_gcPtr->gameOver()) {
            emit(gameResumed());
        }
    }
}

void MainWindow::onBsuiCancelButtonClicked()
{
    this->setEnabled(true);
    this->m_boardSizeDialog->hide();
    this->show();
    if (!this->m_gcPtr->gameOver()) {
        emit(gameResumed());
    }
}

void MainWindow::onAboutQtActionTriggered()
{
    /*
    if ((this->m_gcPtr->gameState() == GameState::GAME_ACTIVE) && (!this->m_gcPtr->gameOver())) {
        this->setEnabled(false);
        this->m_tempPauseFlag = true;
        emit(gamePaused());
    }
    */
}

void MainWindow::onApplicationExit()
{

}

void MainWindow::bindGameController(std::shared_ptr<GameController> gc)
{
    this->m_gcPtr.reset();
    this->m_gcPtr = gc;
}

void MainWindow::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qdw)
{
    this->m_qdwPtr.reset();
    this->m_qdwPtr = qdw;
}

void MainWindow::bindQMineSweeperIcons(std::shared_ptr<QMineSweeperIcons> qmsiPtr)
{
    this->m_qmsiPtr.reset();
    this->m_qmsiPtr = qmsiPtr;
}

void MainWindow::bindQMineSweeperSoundEffects(std::shared_ptr<QMineSweeperSoundEffects> qmssePtr)
{
    this->m_qmssePtr.reset();
    this->m_qmssePtr = qmssePtr;
}

void MainWindow::bindQMineSweeperStrings(std::shared_ptr<QMineSweeperStrings> qmsstrPtr)
{
    this->m_qmsstrPtr.reset();
    this->m_qmsstrPtr = qmsstrPtr;
}

void MainWindow::drawNumberOfSurroundingMines(std::shared_ptr<MineSweeperButton> msb)
{
    if (msb->numberOfSurroundingMines() == 0) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
    } else if (msb->numberOfSurroundingMines() == 1) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_1);
    } else if (msb->numberOfSurroundingMines() == 2) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_2);
    } else if (msb->numberOfSurroundingMines() == 3) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_3);
    } else if (msb->numberOfSurroundingMines() == 4) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_4);
    } else if (msb->numberOfSurroundingMines() == 5) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_5);
    } else if (msb->numberOfSurroundingMines() == 6) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_6);
    } else if (msb->numberOfSurroundingMines() == 7) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_7);
    } else if (msb->numberOfSurroundingMines() == 8) {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_8);
    } else {
        msb->setIcon(this->m_qmsiPtr->COUNT_MINES_0);
    }
}


void MainWindow::onMineExplosionEventTriggered()
{
    using namespace QMineSweeperUtilities;
    displayAllMines();
    this->m_qmssePtr->explosionEffect.play();
    emit(mineExplosionEvent());
}


MainWindow::~MainWindow()
{

}
