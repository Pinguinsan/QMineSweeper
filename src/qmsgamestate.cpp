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


#include "qmsgamestate.h"
#include "qmsstrings.h"
#include "globaldefinitions.h"
#include "qmsbutton.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

const std::pair<double, double> QmsGameState::s_CELL_TO_MINE_RATIOS{std::make_pair(0.15625, 0.15625)};
const int QmsGameState::s_CELL_TO_MINE_THRESHOLD{82};

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


static QmsGameState loadFromFile(const QString &filePath)
{
    return QmsGameState{0, 0};
}

SaveGameStateResult QmsGameState::saveToFile(const QString &filePath)
{
    QFile outputFile{filePath};
    if (outputFile.exists()) {
        if (!outputFile.remove()) {
            return SaveGameStateResult::UnableToDeleteExistingFile;
        }
    }
    if (!outputFile.open(QIODevice::OpenModeFlag::WriteOnly)) {
       return SaveGameStateResult::UnableToOpenFile;
    }
    QXmlStreamWriter writeToFile;
    writeToFile.setDevice(&outputFile);
    writeToFile.setAutoFormatting(true);
    writeToFile.setAutoFormattingIndent(4);
    writeToFile.writeStartDocument();
        writeToFile.writeStartElement("QmsGameState");
            writeToFile.writeTextElement("NumberOfColumns", QS_NUMBER(this->m_numberOfColumns));
            writeToFile.writeTextElement("NumberOfRows", QS_NUMBER(this->m_numberOfRows));
            writeToFile.writeTextElement("NumberOfMines", QS_NUMBER(this->m_numberOfMines));
            writeToFile.writeTextElement("PlayTime", QString::fromStdString(this->m_playTimer->toString()));
            writeToFile.writeStartElement("MineCoordinateList");
                for (auto &it: this->m_mineCoordinates) {
                    writeToFile.writeTextElement("MineCoordinates", QString{MineCoordinates{it}.toString().c_str()});
                }
            writeToFile.writeEndElement(); //MineCoordinates
            writeToFile.writeStartElement("QmsButtons");
                for (auto &it : this->m_mineSweeperButtons) {
                    this->writeQmsButtonToXmlStream(&writeToFile, it.first, it.second);
                }
            writeToFile.writeEndElement(); //MineSweeperButtons
        writeToFile.writeEndElement(); //QmsGameState
    writeToFile.writeEndDocument();
    return SaveGameStateResult::Success;
}

void QmsGameState::writeQmsButtonToXmlStream(QXmlStreamWriter *writeToFile, const MineCoordinates &coordinates, std::shared_ptr<QmsButton> targetButton)
{
    using namespace QmsUtilities;
    writeToFile->writeStartElement("QmsButton");
        writeToFile->writeTextElement("MineCoordinates", QString{coordinates.toString().c_str()});
        writeToFile->writeTextElement("isBlockingClicks", boolToQString(targetButton->isBlockingClicks()));
        writeToFile->writeTextElement("numberOfSurroundingMines", QS_NUMBER(targetButton->numberOfSurroundingMines()));
        writeToFile->writeTextElement("isChecked", boolToQString(targetButton->isChecked()));
        writeToFile->writeTextElement("hasFlag", boolToQString(targetButton->hasFlag()));
        writeToFile->writeTextElement("hasMine", boolToQString(targetButton->hasMine()));
        writeToFile->writeTextElement("isRevealed", boolToQString(targetButton->isRevealed()));
    writeToFile->writeEndElement(); //QmsButton
}
