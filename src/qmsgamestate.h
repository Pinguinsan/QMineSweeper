/***********************************************************************
*    qmsgamestate.h                                                    *
*    Class to hold a saved or loaded game state for QMineSweeper       *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declaration of a QmsGameState class           *
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

#ifndef QMINESWEEPER_QMSGAMESTATE_H
#define QMINESWEEPER_QMSGAMESTATE_H

#include <QObject>
#include <memory>
#include <unordered_map>

#include "minecoordinatehash.h"

class QString;
class QmsButton;
class MineCoordinates;

using ButtonContainer = std::unordered_map<MineCoordinates, std::shared_ptr<QmsButton>, MineCoordinateHash>;

class QmsGameState : public QObject
{
    Q_OBJECT
public:
    explicit QmsGameState(QObject *parent = nullptr);

    int numberOfColumns() const;
    int numberOfRows() const;

    int numberOfMines() const;

    ButtonContainer &mineSweeperButtons();
    std::shared_ptr<QmsButton> mineSweeperButtonAtIndex(const MineCoordinates &coordinates) const;
    std::shared_ptr<QmsButton> mineSweeperButtonAtIndex(int columnIndex, int rowIndex) const;

    static QmsGameState loadFromFile(const QString &filePath);
    static bool saveToFile(const QString &filePath);



signals:

public slots:
};

#endif //QMINESWEEPER_QMSGAMESTATE_H
