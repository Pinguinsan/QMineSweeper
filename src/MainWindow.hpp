#ifndef QMINESWEEPER_MAINWINDOW_HPP
#define QMINESWEEPER_MAINWINDOW_HPP

#include <QMainWindow>
#include <QDialog>
#include <iostream>
#include <memory>

#include "EventTimer.hpp"
#include "QmsSettingsLoader.hpp"
#include "MouseMoveableQMainWindow.hpp"
#include "BoardResizeWidget.hpp"
#include "QmsGameState.hpp"

namespace Ui {
    class MainWindow;
}

class QmsButton;

class GameController;

class AboutQmsWidget;

class QmsIcons;

class QmsSoundEffects;

class QmsApplicationSettings;

class QDesktopWidget;

class QMediaPlayer;

class QDialog;

class QWindow;

class QString;

class QTimer;

class QActionGroup;

class QTranslator;

class QLabel;

class QDialog;

class QPoint;

class QMoveEvent;

class AboutApplicationWidget;

class MainWindow : public MouseMoveableQMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QmsSettingsLoader::SupportedLanguage initialDisplayLanguage,
                        QWidget *parent = nullptr);
    ~MainWindow() override;

    void populateMineField();
    void displayAllMines();
    void resizeResetIcon();
    void displayMineSquare(QmsButton *msb);
    void displayMineFromLoad(QmsButton *msb);
    void setResetButtonIcon(const QIcon &icon);
    void drawNumberOfSurroundingMines(QmsButton *msb);
    void setLanguage(QmsSettingsLoader::SupportedLanguage newLanguage);
    bool boardResizeDialogVisible();

    QmsApplicationSettings collectApplicationSettings() const;
    QString saveStyleSheet() const;

private:
    std::unique_ptr<QTimer> m_eventTimer;
    std::unique_ptr<SteadyEventTimer> m_userIdleTimer;
    std::unique_ptr<AboutApplicationWidget> m_aboutQmsDialog;
    std::unique_ptr<BoardResizeWidget> m_boardResizeDialog;
    std::unique_ptr<QActionGroup> m_languageActionGroup;
    std::unique_ptr<QTranslator> m_translator;
    std::unique_ptr<QLabel> m_statusBarLabel;
    QmsSettingsLoader::SupportedLanguage m_language;

    double m_reductionSizeScaleFactor;
    QString m_saveStyleSheet;
    QSize m_currentDefaultMineSize;
    QSize m_currentMaxMineSize;
    QSize m_currentIconReductionSize;
    bool m_maxMineSizeCacheIsValid;
    bool m_iconReductionSizeCacheIsValid;
    bool m_boardSizeGeometrySet;
    QString m_saveFilePath;
    Ui::MainWindow *m_ui;

    static const int TASKBAR_HEIGHT;
    static const int GAME_TIMER_INTERVAL;
    static const double DEFAULT_NUMBER_OF_MINES;
    static const int HEIGHT_SCALE_FACTOR;
    static const int WIDTH_SCALE_FACTOR;
    static const int NUMBER_OF_HORIZONTAL_MARGINS;
    static const int NUMBER_OF_VERTIAL_MARGINS;
    static const int DEFAULT_MINE_SIZE_SCALE_FACTOR;
    static const int STATUS_BAR_FONT_POINT_SIZE;
    static const double MINE_ICON_REDUCTION_SCALE_FACTOR;

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void invalidateSizeCaches();
    void doGameReset();
    QSize getMaxMineSize();
    QSize getIconReductionSize();
    std::string getLCDPadding(uint8_t howMuch);
    static const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    static const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    static const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    static const long long int constexpr MILLISECONDS_PER_HOUR{3600000};

    void displayStatusMessage(QString statusMessage);
    void doSaveGame(const QString &filePath);
signals:
    void resetButtonClicked();
    void resetGame();
    void mineExplosionEvent();
    void boardResize(int columns, int rows);
    void gamePaused();
    void gameResumed();
    void mineDisplayed();
    void winEvent();
    void mineSweeperButtonCreated(std::shared_ptr<QmsButton> msb);

public slots:
    void resetResetButtonIcon();
    void onGameStarted();
    void onGamePaused();
    void onGameResumed();
    void onMineExplosionEventTriggered();
    void setupNewGame();
    void onGameWon();
    void onBoardResizeDialogClosed(BoardResizeWidget::ResizeWidgetResult result);
    void startUserIdleTimer();

private slots:
    void eventLoop();
    void onResetButtonClicked();
    void onAboutQtActionTriggered();
    void onAboutQMineSweeperActionTriggered();
    void onAboutQmsWindowClosed();
    void onApplicationExit();
    void updateVisibleGameTimer();
    void updateUserIdleTimer();
    void startGameTimer();
    void onChangeBoardSizeActionTriggered();
    void onCustomMineRatioSet(float mineRatio);

    void onActionMuteSoundChecked(bool checked);

    void onLanguageSelected(bool triggered);
    void onSaveActionTriggered();
    void onSaveAsActionTriggered();
    void onOpenActionTriggered();
    void updateMyGeometry();

    void onLoadGameCompleted(const std::pair<LoadGameStateResult, std::string> &loadResult,
                             const QmsGameState &gameState);
};

#endif // QMINESWEEPER_MAINWINDOW_HPP
