/***********************************************************************
*    qmssoundeffects.h:                                                *
*    Class for holding instances of all sounds used in QMineSweeper    *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QmsSoundEffects             *
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

#ifndef QMINESWEEPER_QMSSOUNDEFFECTS_H
#define QMINESWEEPER_QMSSOUNDEFFECTS_H


#include <QMediaPlayer>
class QString;

#include "QmsUtilities.h"
#include "QmsStrings.h"

class QmsSoundEffects
{
public:
    void setAudioVolume(int volume);
    void setAudioMuted(bool soundMuted);
    int restoreVolumeFromMute();
    int audioVolume() const;
    bool isMuted() const;
    QMediaPlayer &explosionEffect();

    static void initializeInstance(int audioVolume = 0);
private:
    QMediaPlayer m_explosionEffect;
    QString m_explosionEffectSource;
    int m_audioVolume;
    int m_storedMuteVolume;

    static const int s_DEFAULT_EXPLOSION_EFFECT_VOLUME;
    static const int s_MAXIMUM_SOUND_VOLUME;

    explicit QmsSoundEffects(int audioVolume = 0);
    ~QmsSoundEffects();
};

extern QmsSoundEffects *applicationSoundEffects;

#endif //QMINESWEEPER_QMSSOUNDEFFECTS_H
