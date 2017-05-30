/***********************************************************************
*    qminesweepersoundeffects.h:                                       *
*    Class for holding instances of all sounds used in QMineSweeper    *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QMineSweeperSoundEffects    *
*    class. QMineSweeperSoundEffects holds instances of all sound      *
*    effects used in the game, allowing members to be called to        *
*    play sounds, rather than having to use a raw filename string to   *
*    access the sound effect on disk, thus allowing low-latency effects*
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMINESWEEPERSOUNDEFFECTS_H
#define QMINESWEEPER_QMINESWEEPERSOUNDEFFECTS_H

#include <QSoundEffect>
#include <QMediaPlayer>
#include <QString>
#include <QUrl>

#include "qminesweeperutilities.h"
#include "qminesweeperstrings.h"

class QMineSweeperSoundEffects
{
public:
    QMineSweeperSoundEffects();
    ~QMineSweeperSoundEffects();

    QMediaPlayer explosionEffect;

private:
    QString m_explosionEffectSource;

    static const int s_EXPLOSION_EFFECT_VOLUME;

};

#endif //QMINESWEEPER_QMINESWEEPERSOUNDEFFECTS_H
