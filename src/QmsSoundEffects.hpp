#ifndef QMINESWEEPER_QMSSOUNDEFFECTS_HPP
#define QMINESWEEPER_QMSSOUNDEFFECTS_HPP


#include <QMediaPlayer>
class QString;

#include "QmsUtilities.hpp"
#include "QmsStrings.hpp"

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

#endif //QMINESWEEPER_QMSSOUNDEFFECTS_HPP
