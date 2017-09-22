/***********************************************************************
*    qmsgamestate.cpp                                                  *
*    Class to hold a saved or loaded game state for QMineSweeper       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsGameState class        *
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


#include "QmsGameState.h"
#include "QmsStrings.h"
#include "GlobalDefinitions.h"
#include "QmsButton.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileInfo>

const std::pair<double, double> QmsGameState::s_CELL_TO_MINE_RATIOS{std::make_pair(0.15625, 0.15625)};
const int QmsGameState::s_CELL_TO_MINE_THRESHOLD{82};

QmsGameState::QmsGameState() :
    QmsGameState{0, 0}
{

}

QmsGameState::QmsGameState(int columnCount, int rowCount) :
    m_playTimer{new SteadyEventTimer{}},
    m_numberOfMines{0},
    m_userDisplayNumberOfMines{0},
    m_initialClickFlag{true},
    m_numberOfColumns{columnCount},
    m_numberOfRows{rowCount},
    m_numberOfMovesMade{0},
    m_gameState{GameState::GameInactive},
    m_gameOver{false},
    m_totalButtonCount{this->m_numberOfColumns*this->m_numberOfRows},
    m_unopenedMineCount{this->m_numberOfColumns*this->m_numberOfRows}

{
    using namespace QmsUtilities;
    this->m_numberOfMines = ((this->m_numberOfColumns * this->m_numberOfRows) < this->s_CELL_TO_MINE_THRESHOLD) ?
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.first)) :
            static_cast<int>(roundIntuitively(static_cast<double>(this->m_numberOfColumns) * static_cast<double>(this->m_numberOfRows) * s_CELL_TO_MINE_RATIOS.second));
    this->m_userDisplayNumberOfMines = this->m_numberOfMines;
}


QmsGameState::~QmsGameState()
{

}


LoadGameStateResult QmsGameState::loadGameInPlace(const QString &filePath)
{
    QmsGameState loadedState;
    auto result = QmsGameState::loadFromFile(filePath, loadedState);
    if (result == LoadGameStateResult::Success) {
        //Save state to this
    }
    return result;
}

LoadGameStateResult QmsGameState::loadFromFile(const QString &filePath, QmsGameState &targetState)
{
    Q_UNUSED(targetState);
    using namespace QmsUtilities;
    QFile inputFile{filePath};
    QXmlStreamReader readFromFile{};
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
    readFromFile.setDevice(&inputFile);
    inputFile.seek(0);
    while (!readFromFile.atEnd() && !readFromFile.hasError()) {
        readFromFile.readNext();
        if (readFromFile.isStartElement()) {
            LOG_DEBUG() << QString{"element name: %1, text: %2"}.arg(readFromFile.name().toString(), readFromFile.readElementText(QXmlStreamReader::ReadElementTextBehaviour::SkipChildElements));
        }
    }
    if (readFromFile.hasError()) {
        LOG_CRITICAL() << QString{"An XML parsing error occurred: %1"}.arg(readFromFile.errorString());
    }
    inputFile.close();

    return LoadGameStateResult::Success; //QmsGameState{0, 0};
}


SaveGameStateResult QmsGameState::saveToFile(const QString &filePath)
{
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
    auto flaggedMines = std::count_if(this->m_mineSweeperButtons.begin(), this->m_mineSweeperButtons.end(), [](auto container) { return container.second->hasFlag(); });
    auto numberOfMinesRemaining = this->m_numberOfMines - flaggedMines;
    QXmlStreamWriter writeToFile{};
    writeToFile.setDevice(&outputFile);
    writeToFile.setAutoFormatting(true);
    writeToFile.setAutoFormattingIndent(4);
    writeToFile.writeStartDocument();
        writeToFile.writeStartElement("QmsGameState");
            writeToFile.writeTextElement("NumberOfColumns", QS_NUMBER(this->m_numberOfColumns));
            writeToFile.writeTextElement("NumberOfRows", QS_NUMBER(this->m_numberOfRows));
            writeToFile.writeTextElement("NumberOfMines", QS_NUMBER(this->m_numberOfMines));
            writeToFile.writeTextElement("NumberOfMovesMade", QS_NUMBER(this->m_numberOfMovesMade));
            writeToFile.writeTextElement("NumberOfMinesRemaining", QS_NUMBER(numberOfMinesRemaining));
            writeToFile.writeStartElement("PlayTime");
                writeToFile.writeTextElement("IsPaused", boolToQString(this->m_playTimer->m_isPaused));
                writeToFile.writeTextElement("IsStopped", boolToQString(this->m_playTimer->m_isStopped));
                writeToFile.writeTextElement("TotalTime", QS_NUMBER(this->m_playTimer->m_totalTime));
            writeToFile.writeEndElement(); //PlayTime
            writeToFile.writeStartElement("MineCoordinateList");
                for (auto &it: this->m_mineCoordinates) {
                    writeToFile.writeTextElement("MineCoordinates", QString{MineCoordinates{it}.toString().c_str()});
                }
            writeToFile.writeEndElement(); //MineCoordinates
            writeToFile.writeStartElement("QmsButtons");
                for (auto &it : this->m_mineSweeperButtons) {
                    auto coordinates = it.first;
                    auto targetButton = it.second;
                    writeToFile.writeStartElement("QmsButton");
                        writeToFile.writeTextElement("MineCoordinates", QString{coordinates.toString().c_str()});
                        writeToFile.writeTextElement("IsBlockingClicks", boolToQString(targetButton->isBlockingClicks()));
                        writeToFile.writeTextElement("NumberOfSurroundingMines", QS_NUMBER(targetButton->numberOfSurroundingMines()));
                        writeToFile.writeTextElement("IsChecked", boolToQString(targetButton->isChecked()));
                        writeToFile.writeTextElement("HasFlag", boolToQString(targetButton->hasFlag()));
                        writeToFile.writeTextElement("HasMine", boolToQString(targetButton->hasMine()));
                        writeToFile.writeTextElement("IsRevealed", boolToQString(targetButton->isRevealed()));
                    writeToFile.writeEndElement(); //QmsButton
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
