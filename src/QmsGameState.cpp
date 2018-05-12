#include "QmsGameState.hpp"
#include "QmsStrings.hpp"
#include "GlobalDefinitions.hpp"
#include "QmsButton.hpp"
#include "QmsUtilities.hpp"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileInfo>

using namespace QmsStrings;

const std::pair<double, double> QmsGameState::s_CELL_TO_MINE_RATIOS{std::make_pair(0.15625, 0.15625)};
const int QmsGameState::s_CELL_TO_MINE_THRESHOLD{82};

QmsGameState::QmsGameState() :
    QmsGameState{0, 0}
{

}

QmsGameState::QmsGameState(int columnCount, int rowCount) :
    m_playTimer{},
    m_numberOfMines{0},
    m_userDisplayNumberOfMines{0},
    m_initialClickFlag{true},
    m_numberOfColumns{columnCount},
    m_numberOfRows{rowCount},
    m_numberOfMovesMade{0},
    m_gameState{GameState::GameInactive},
    m_gameOver{false},
    m_totalButtonCount{this->m_numberOfColumns*this->m_numberOfRows},
    m_unopenedMineCount{this->m_numberOfColumns*this->m_numberOfRows},
    m_customMineRatio{nullptr},
    m_filePath{""}

{
    using namespace QmsUtilities;
    this->m_numberOfMines = ((this->m_numberOfColumns * this->m_numberOfRows) < this->s_CELL_TO_MINE_THRESHOLD) ?
            roundIntuitively(this->m_numberOfColumns * this->m_numberOfRows * s_CELL_TO_MINE_RATIOS.first) :
            roundIntuitively(this->m_numberOfColumns * this->m_numberOfRows * s_CELL_TO_MINE_RATIOS.second);
	this->m_userDisplayNumberOfMines = this->m_numberOfMines;
}

QmsGameState::QmsGameState(const QmsGameState &rhs) :
	m_playTimer{rhs.m_playTimer},
	m_numberOfMines{rhs.m_numberOfMines},
	m_userDisplayNumberOfMines{rhs.m_userDisplayNumberOfMines},
	m_initialClickFlag{rhs.m_initialClickFlag},
	m_numberOfColumns{rhs.m_numberOfColumns},
	m_numberOfRows{rhs.m_numberOfRows},
    m_numberOfMovesMade{rhs.m_numberOfMovesMade},
    m_gameState{rhs.m_gameState},
    m_gameOver{rhs.m_gameOver},
	m_totalButtonCount{rhs.m_totalButtonCount},
	m_unopenedMineCount{ rhs.m_unopenedMineCount },
	m_customMineRatio{nullptr},
    m_filePath{rhs.m_filePath}
	

{
	if (rhs.m_customMineRatio) {
		this->m_customMineRatio.reset(new float{ *rhs.m_customMineRatio });
	}
    this->m_mineCoordinates.clear();
    for (const auto &it : rhs.m_mineCoordinates) {
        this->m_mineCoordinates.emplace(it);
    }

    this->m_mineSweeperButtons.clear();
    for (const auto &it : rhs.m_mineSweeperButtons) {
        this->m_mineSweeperButtons.emplace(it);
    }

}

QmsGameState::QmsGameState(QmsGameState &&rhs) noexcept :
	m_playTimer{ std::move( rhs.m_playTimer) },
	m_numberOfMines{ rhs.m_numberOfMines },
	m_userDisplayNumberOfMines{ rhs.m_userDisplayNumberOfMines },
	m_initialClickFlag{ rhs.m_initialClickFlag },
	m_numberOfColumns{ rhs.m_numberOfColumns },
	m_numberOfRows{ rhs.m_numberOfRows },
    m_numberOfMovesMade{rhs.m_numberOfMovesMade},
    m_gameState{ rhs.m_gameState },
	m_gameOver{ rhs.m_gameOver },
	m_totalButtonCount{ rhs.m_totalButtonCount },
	m_unopenedMineCount{ rhs.m_unopenedMineCount },
	m_customMineRatio{ std::move(rhs.m_customMineRatio ) },
    m_filePath{rhs.m_filePath}


{
    this->m_mineCoordinates.clear();
    for (const auto &it : rhs.m_mineCoordinates) {
        this->m_mineCoordinates.emplace(it);
    }

    this->m_mineSweeperButtons.clear();
    for (const auto &it : rhs.m_mineSweeperButtons) {
        this->m_mineSweeperButtons.emplace(it);
    }

}

QString QmsGameState::filePath() const {
    return this->m_filePath;
}

QmsGameState &QmsGameState::operator=(const QmsGameState &rhs)
{
    this->m_mineCoordinates.clear();
    for (const auto &it : rhs.m_mineCoordinates) {
        this->m_mineCoordinates.emplace(it);
    }

    this->m_mineSweeperButtons.clear();
    for (const auto &it : rhs.m_mineSweeperButtons) {
        this->m_mineSweeperButtons.emplace(it);
    }

	this->m_playTimer = rhs.m_playTimer;
	this->m_numberOfMines = rhs.m_numberOfMines;
	this->m_userDisplayNumberOfMines = rhs.m_userDisplayNumberOfMines;
	this->m_initialClickFlag = rhs.m_initialClickFlag;
	this->m_numberOfColumns = rhs.m_numberOfColumns;
	this->m_numberOfRows = rhs.m_numberOfRows;
	this->m_gameState = rhs.m_gameState;
	this->m_gameOver = rhs.m_gameOver;
	this->m_totalButtonCount = rhs.m_totalButtonCount;
	this->m_unopenedMineCount = rhs.m_unopenedMineCount;
	if (rhs.m_customMineRatio) {
		this->m_customMineRatio.reset(new float{ *rhs.m_customMineRatio });
	}
    this->m_filePath = rhs.m_filePath;
	return *this;
}

QmsGameState &QmsGameState::operator=(QmsGameState &&rhs)
{
    this->m_mineCoordinates.clear();
    for (const auto &it : rhs.m_mineCoordinates) {
        this->m_mineCoordinates.emplace(it);
    }

    this->m_mineSweeperButtons.clear();
    for (const auto &it : rhs.m_mineSweeperButtons) {
        this->m_mineSweeperButtons.emplace(it);
    }


    this->m_playTimer = rhs.m_playTimer;
	this->m_numberOfMines = rhs.m_numberOfMines;
	this->m_userDisplayNumberOfMines = rhs.m_userDisplayNumberOfMines;
	this->m_initialClickFlag = rhs.m_initialClickFlag;
	this->m_numberOfColumns = rhs.m_numberOfColumns;
	this->m_numberOfRows = rhs.m_numberOfRows;
	this->m_gameState = rhs.m_gameState;
	this->m_gameOver = rhs.m_gameOver;
	this->m_totalButtonCount = rhs.m_totalButtonCount;
	this->m_unopenedMineCount = rhs.m_unopenedMineCount;
	this->m_customMineRatio = std::move(rhs.m_customMineRatio);
    this->m_filePath = rhs.m_filePath;

    return *this;
}


LoadGameStateResult QmsGameState::loadGameInPlace(const QString &filePath)
{
    QmsGameState loadedState;
	const auto result = QmsGameState::loadFromFile(filePath, loadedState);
    if (result == LoadGameStateResult::Success) {
		*this = loadedState;
    }
    return result;
}

LoadGameStateResult QmsGameState::loadFromFile(const QString &filePath, QmsGameState &targetState)
{
    Q_UNUSED(targetState);
    using namespace QmsUtilities;
    QFile inputFile{filePath};
    QXmlStreamReader reader{};
    if (!inputFile.exists()) {
        return LoadGameStateResult::FileDoesNotExist;
    }
    if (!inputFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
        return LoadGameStateResult::UnableToOpenFile;
    }
    auto fileHash = QmsUtilities::getFileChecksum(&inputFile, QCryptographicHash::Sha512);
    QFile hashFile{QString{"%1.%2.hash"}.arg(getFileDirectoryPath(inputFile), getFileName(inputFile))};
    if (!hashFile.exists()) {
        return LoadGameStateResult::HashFileDoesNotExist;
    }
    if (!hashFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
        return LoadGameStateResult::UnableToOpenHashFile;
    }
    auto savedHash = hashFile.readAll();
    hashFile.close();
    if (fileHash != savedHash) {
        return LoadGameStateResult::HashVerificationFailed;
    }
    reader.setDevice(&inputFile);
    inputFile.seek(0);
    targetState.m_filePath = filePath;

    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.name().isEmpty()) {
            continue;
        }
        LOG_DEBUG() << QString{"reader.name() = %1"}.arg(reader.name().toString());
        if (reader.name() == QMS_GAME_STATE_XML_KEY) {
            //Opening element
            continue;
        } else if (reader.name() == COLUMN_COUNT_XML_KEY) {
            targetState.m_numberOfColumns = reader.readElementText().toInt();
        } else if (reader.name() == ROW_COUNT_XML_KEY) {
            targetState.m_numberOfRows = reader.readElementText().toInt();
        } else if (reader.name() == MINE_COUNT_XML_KEY) {
            targetState.m_numberOfMines = reader.readElementText().toInt();
        } else if (reader.name() == MOVES_MADE_COUNT_XML_KEY) {
            targetState.m_numberOfMovesMade = reader.readElementText().toInt();
        } else if (reader.name() == MINES_REMAINING_COUNT_XML_KEY) {
            targetState.m_unopenedMineCount = reader.readElementText().toInt();
        } else if (reader.name() == PLAY_TIMER_START_ELEMENT_XML_KEY) {
            targetState.m_playTimer = readEventTimerFromXmlFile(reader);
        } else if (reader.name() == MINE_COORDINATE_LIST_XML_KEY) {
            auto coordinateList = readMineCoordinateListFromXmlFile(reader);
            for (const auto &it : coordinateList) {
                targetState.m_mineCoordinates.insert(it);
            }
        } else if (reader.name() == QMS_BUTTON_LIST_START_ELEMENT_XML_KEY) {
            auto buttonList = readQmsButtonListFromXmlFile(reader);
            for (const auto &it : buttonList) {
                targetState.m_mineSweeperButtons.insert(it);
            }
        }
        //LOG_DEBUG() << QString{"element name: %1, text: %2"}.arg(reader.name().toString(), reader.readElementText(QXmlStreamReader::ReadElementTextBehaviour::IncludeChildElements));
    }
    if (reader.hasError()) {
        LOG_CRITICAL() << QString{"An XML parsing error occurred: %1"}.arg(reader.errorString());
        inputFile.close();
        return LoadGameStateResult::XmlParseFailed;
    }
    inputFile.close();

    return LoadGameStateResult::Success;
}

std::list<MineCoordinates> QmsGameState::readMineCoordinateListFromXmlFile(QXmlStreamReader &reader)
{
    //QMS_BUTTON_LIST_START_ELEMENT_XML_KEY
    auto returnList = std::list<MineCoordinates>{};
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.name().isEmpty()) {
            continue;
        }
        if (reader.name() == QMS_BUTTON_MINE_COORDINATES_XML_KEY) {
            returnList.push_back(MineCoordinates::parse(reader.readElementText().toStdString()));
        } else {
            break;
        }
    }
    return returnList;
}

std::list<std::pair<MineCoordinates, std::shared_ptr<QmsButton>>> QmsGameState::readQmsButtonListFromXmlFile(QXmlStreamReader &reader)
{
    auto returnList = std::list<std::pair<MineCoordinates, std::shared_ptr<QmsButton>>>{};
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.name().isEmpty()) {
            continue;
        }
        if (reader.name() == QMS_BUTTON_START_ELEMENT_XML_KEY) {
            returnList.push_back(readQmsButtonFromXmlFile(reader));
        } else {
            break;
        }
    }
    return returnList;
}

std::pair<MineCoordinates, std::shared_ptr<QmsButton>> QmsGameState::readQmsButtonFromXmlFile(QXmlStreamReader &reader)
{
    using namespace QmsUtilities;
    MineCoordinates coordinates{0, 0};
    std::shared_ptr<QmsButton> button{std::make_shared<QmsButton>(0, 0, nullptr)};
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.name().isEmpty()) {
            continue;
        }
        QString elementText{reader.readElementText()};
        if (reader.name() == QMS_BUTTON_MINE_COORDINATES_XML_KEY) {
            coordinates = MineCoordinates::parse(elementText.toStdString());
            button->setCoordinates(coordinates);
        } else if (reader.name() == QMS_BUTTON_IS_BLOCKING_CLICKS_XML_KEY) {
            button->setBlockClicks(toBool(elementText));
        } else if (reader.name() == QMS_BUTTON_SURROUNDING_MINE_COUNT_XML_KEY) {
            button->setNumberOfSurroundingMines(elementText.toInt());
        } else if (reader.name() == QMS_BUTTON_IS_CHECKED_XML_KEY) {
            button->setChecked(toBool(elementText));
        } else if (reader.name() == QMS_BUTTON_HAS_FLAG_XML_KEY) {
            button->setHasFlag(toBool(elementText));
        } else if (reader.name() == QMS_BUTTON_HAS_MINE_XML_KEY) {
            button->setHasMine(toBool(elementText));
        } else if (reader.name() == QMS_BUTTON_IS_REVEALED_XML_KEY) {
            button->setChecked(toBool(elementText));
            button->setIsRevealed(toBool(elementText));
        } else {
            break;
        }
    }
    //std::cout << "coordinates = " << coordinates.toString() << ", QmsButton = " << button->toString() << std::endl;
    return std::make_pair(coordinates, button);
}

SteadyEventTimer QmsGameState::readEventTimerFromXmlFile(QXmlStreamReader &reader)
{
    using QmsUtilities::toBool;
    SteadyEventTimer eventTimer{};
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.name().isEmpty()) {
            continue;
        }
        QString elementText{reader.readElementText()};
        if (reader.name() == PLAY_TIMER_IS_PAUSED_XML_KEY) {
            bool isPaused{toBool(elementText)};
            if (isPaused) {
                eventTimer.pause();
            } else {
                eventTimer.start();
            }
        } else if (reader.name() == PLAY_TIMER_TOTAL_TIME_XML_KEY) {
            eventTimer.setTotalTime(elementText.toInt());
        } else {
            break;
        }
    }
    return eventTimer;
}

SaveGameStateResult QmsGameState::saveToFile(const QString &filePath)
{
    this->m_filePath = filePath;
    QFile outputFile{filePath};
    if (outputFile.exists()) {
        if (!outputFile.remove()) {
            return SaveGameStateResult::UnableToDeleteExistingFile;
        }
    }
    if (!outputFile.open(QIODevice::OpenModeFlag::ReadWrite)) {
       return SaveGameStateResult::UnableToOpenFile;
    }
    using namespace QmsUtilities;
    //auto flaggedMines = std::count_if(this->m_mineSweeperButtons.begin(), this->m_mineSweeperButtons.end(), [](auto container) { return container.second->hasFlag(); });
    //auto numberOfMinesRemaining = this->m_numberOfMines - flaggedMines;
    QXmlStreamWriter writeToFile{};
    writeToFile.setDevice(&outputFile);
    writeToFile.setAutoFormatting(true);
    writeToFile.setAutoFormattingIndent(4);
    writeToFile.writeStartDocument();
        writeToFile.writeStartElement(QMS_GAME_STATE_XML_KEY);
            writeToFile.writeTextElement(COLUMN_COUNT_XML_KEY, QS_NUMBER(this->m_numberOfColumns));
            writeToFile.writeTextElement(ROW_COUNT_XML_KEY, QS_NUMBER(this->m_numberOfRows));
            writeToFile.writeTextElement(MINE_COUNT_XML_KEY, QS_NUMBER(this->m_numberOfMines));
            writeToFile.writeTextElement(MOVES_MADE_COUNT_XML_KEY, QS_NUMBER(this->m_numberOfMovesMade));
            writeToFile.writeTextElement(MINES_REMAINING_COUNT_XML_KEY, QS_NUMBER(this->m_unopenedMineCount));

            writeToFile.writeStartElement(PLAY_TIMER_START_ELEMENT_XML_KEY);
                writeToFile.writeTextElement(PLAY_TIMER_IS_PAUSED_XML_KEY, boolToQString(this->m_playTimer.m_isPaused));
                writeToFile.writeTextElement(PLAY_TIMER_TOTAL_TIME_XML_KEY, QS_NUMBER(this->m_playTimer.m_totalTime));
            writeToFile.writeEndElement(); //PlayTime

            writeToFile.writeStartElement(MINE_COORDINATE_LIST_XML_KEY);
                for (auto &it: this->m_mineCoordinates) {
                    writeToFile.writeTextElement(MINE_COORDINATES_XML_KEY, QString{MineCoordinates{it}.toString().c_str()});
                }
            writeToFile.writeEndElement(); //MineCoordinates

            writeToFile.writeStartElement(QMS_BUTTON_LIST_START_ELEMENT_XML_KEY);
                for (auto &it : this->m_mineSweeperButtons) {
					writeQmsButtonToXmlStream(writeToFile, it.first, it.second);
                }
            writeToFile.writeEndElement(); //MineSweeperButtons
        writeToFile.writeEndElement(); //QmsGameState
    writeToFile.writeEndDocument();
    outputFile.seek(0);
    auto fileHash = QmsUtilities::getFileChecksum(&outputFile, QCryptographicHash::Sha512);
    QFile hashFile{QString{"%1.%2.hash"}.arg(getFileDirectoryPath(outputFile), getFileName(outputFile))};
    if (hashFile.exists()) {
        if (!hashFile.remove()) {
            if (!outputFile.remove()) {
                return SaveGameStateResult::UnableToDeleteExistingFile;
            } else {
                return SaveGameStateResult::UnableToDeleteExistingHashFile;
            }
        }
    }
    if (!hashFile.open(QIODevice::OpenModeFlag::WriteOnly)) {
        if (!hashFile.remove()) {
            if (!outputFile.remove()) {
                return SaveGameStateResult::UnableToDeleteExistingFile;
            } else {
                return SaveGameStateResult::UnableToDeleteExistingHashFile;
            }
        } else {
            return SaveGameStateResult::UnableToOpenFileToWriteHash;
        }
    }
    hashFile.write(fileHash);
    hashFile.close();
    outputFile.close();
    return SaveGameStateResult::Success;
}



void QmsGameState::writeQmsButtonToXmlStream(QXmlStreamWriter &writeToFile, const MineCoordinates &coordinates,
                                             std::shared_ptr<QmsButton> targetButton)
{
	using namespace QmsUtilities;
	writeToFile.writeStartElement(QMS_BUTTON_START_ELEMENT_XML_KEY);
	writeToFile.writeTextElement(QMS_BUTTON_MINE_COORDINATES_XML_KEY, QString{ coordinates.toString().c_str() });
	writeToFile.writeTextElement(QMS_BUTTON_IS_BLOCKING_CLICKS_XML_KEY, boolToQString(targetButton->isBlockingClicks()));
	writeToFile.writeTextElement(QMS_BUTTON_SURROUNDING_MINE_COUNT_XML_KEY, QS_NUMBER(targetButton->numberOfSurroundingMines()));
	writeToFile.writeTextElement(QMS_BUTTON_IS_CHECKED_XML_KEY, boolToQString(targetButton->isChecked()));
	writeToFile.writeTextElement(QMS_BUTTON_HAS_FLAG_XML_KEY, boolToQString(targetButton->hasFlag()));
	writeToFile.writeTextElement(QMS_BUTTON_HAS_MINE_XML_KEY, boolToQString(targetButton->hasMine()));
	writeToFile.writeTextElement(QMS_BUTTON_IS_REVEALED_XML_KEY, boolToQString(targetButton->isRevealed()));
	writeToFile.writeEndElement(); //QmsButton
}
