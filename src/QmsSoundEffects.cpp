/***********************************************************************
*    qmssoundeffects.cpp:                                              *
*    Class for holding instances of all sounds used in QMineSweeper    *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsSoundEffects           *
*    class. QmsSoundEffects holds instances of all sound               *
*    effects used in the game, allowing members to be called to        *
*    play sounds, rather than having to use a raw filename string to   *
*    access the sound effect on disk, thus allowing low-latency effects*
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "QmsSoundEffects.h"
#include "QmsUtilities.h"
#include "GlobalDefinitions.h"

#include <limits>
#include <exception>
#include <stdexcept>

#include <QSoundEffect>
#include <QString>
#include <QUrl>
#include <QMediaPlayer>

using namespace QmsStrings;

const int QmsSoundEffects::s_MAXIMUM_SOUND_VOLUME{std::numeric_limits<uint16_t>::max()};

/* QmsSoundEffects() : Constructor, setting up the QSoundEffects used.
 * This thin wrapper class for the QSoundEffects is necessary because all QObjects
 * must be instantiated and set up after a QApplication is instantiated, so using them
 * as static objects is not usable. Thus, this late binding is necessary */
QmsSoundEffects::QmsSoundEffects(int audioVolume) :
    m_explosionEffect{},
    m_explosionEffectSource{EXPLOSION_EFFECT_SOURCE_PATH},
    m_audioVolume{audioVolume},
    m_storedMuteVolume{-1}
{
    this->m_explosionEffect.setMedia(QUrl{this->m_explosionEffectSource});
    this->m_explosionEffect.setVolume(this->m_audioVolume);
}

QmsSoundEffects::QmsSoundEffects() :
    QmsSoundEffects{0}
{

}

QMediaPlayer &QmsSoundEffects::explosionEffect()
{
    return this->m_explosionEffect;
}


void QmsSoundEffects::setAudioVolume(int volume)
{
    using namespace QmsUtilities;
    if (volume < 0) {
        throw std::runtime_error(QString{"In QmsSoundEffects::setSoundVolume(int): volume cannot be negative (%1 < 0)"}.arg(QS_NUMBER(volume)).toStdString());
    } else if (volume > s_MAXIMUM_SOUND_VOLUME) {
        throw std::runtime_error(QString{"In QmsSoundEffects::setSoundVolume(int): volume cannot be greater than maximum sound volume (%1 > %2)"}.arg(volume, s_MAXIMUM_SOUND_VOLUME).toStdString());
    } else {
        if (volume == 0) {
            this->m_storedMuteVolume = this->m_audioVolume;
        }
        this->m_audioVolume = volume;
    }
}

int QmsSoundEffects::restoreVolumeFromMute()
{
    if (this->m_storedMuteVolume == -1) {
        return -1;
    } else {
        return (this->m_audioVolume = this->m_storedMuteVolume);
    }
}

bool QmsSoundEffects::isMuted() const
{
    return this->m_audioVolume == 0;
}

int QmsSoundEffects::audioVolume() const
{
    return this->m_audioVolume;
}

void QmsSoundEffects::setAudioMuted(bool audioMuted)
{
    if (audioMuted) {
        this->m_storedMuteVolume = this->m_audioVolume;
        this->m_audioVolume = 0;
    } else {
        this->restoreVolumeFromMute();
    }
}

/* ~QmsSoundEffects() : Destructor, empty by defaut */
QmsSoundEffects::~QmsSoundEffects()
{
    //Destructor
}
