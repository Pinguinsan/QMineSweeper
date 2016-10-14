/***********************************************************************
*    qminesweepersoundeffects.cpp:                                     *
*    Class for holding instances of all sounds used in QMineSweeper    *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation of a QMineSweeperSoundEffects  *
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

#include "qminesweepersoundeffects.h"

QMineSweeperSoundEffects::QMineSweeperSoundEffects() :
    explosionEffect{},
    m_explosionEffectSource{"qrc:/sounds/sounds/explosion-fast.wav"}
{
    this->explosionEffect.setMedia(QUrl{this->m_explosionEffectSource});
    this->explosionEffect.setVolume(75);
}

QMineSweeperSoundEffects::~QMineSweeperSoundEffects()
{
    //Destructor
}
