#ifndef QMINESWEEPER_QMSGAMESTATE_HPP
#define QMINESWEEPER_QMSGAMESTATE_HPP

#include <memory>
#include <set>
#include <utility>
#include <unordered_map>
#include <chrono>
#include <map>
#include <QtCore/QXmlStreamReader>

#include "MineCoordinateHash.hpp"
#include "EventTimer.hpp"
#include "ChangeAwareValue.hpp"

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
    XmlParseFailed,
    UnableToOpenFile,
    HashFileDoesNotExist,
    UnableToOpenHashFile,
    HashVerificationFailed
};

class QmsGameState {
    friend class GameController;

public:
    QmsGameState();
    QmsGameState(int columnCount, int rowCount);
    QmsGameState(const QmsGameState &);
    QmsGameState(QmsGameState &&rhs) noexcept;
    QmsGameState &operator=(const QmsGameState &);
    QmsGameState &operator=(QmsGameState &&rhs);
    ~QmsGameState() = default;

    LoadGameStateResult loadGameInPlace(const QString &filePath);
    SaveGameStateResult saveToFile(const QString &filePath);

    QString filePath() const;

private:
    SteadyEventTimer m_playTimer;
    std::set<MineCoordinates> m_mineCoordinates;
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
    QString m_filePath;

    void writeQmsButtonToXmlStream(QXmlStreamWriter &writeToFile, const MineCoordinates &coordinates,
                                   std::shared_ptr<QmsButton> targetButton);

    static const std::pair<double, double> s_CELL_TO_MINE_RATIOS;
    static const int s_CELL_TO_MINE_THRESHOLD;

    static LoadGameStateResult loadFromFile(const QString &filePath, QmsGameState &targetState);
    static std::pair<MineCoordinates, std::shared_ptr<QmsButton>> readQmsButtonFromXmlFile(QXmlStreamReader &reader);
    static std::list<std::pair<MineCoordinates, std::shared_ptr<QmsButton>>>
    readQmsButtonListFromXmlFile(QXmlStreamReader &reader);
    static SteadyEventTimer readEventTimerFromXmlFile(QXmlStreamReader &reader);
    static std::list<MineCoordinates> readMineCoordinateListFromXmlFile(QXmlStreamReader &reader);

};

#endif //QMINESWEEPER_QMSGAMESTATE_HPP
