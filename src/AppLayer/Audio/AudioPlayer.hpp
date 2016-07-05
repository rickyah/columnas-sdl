//
//  AudioPlayer.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 23/6/16.
//
//

#ifndef AudioPlayer_hpp
#define AudioPlayer_hpp

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "Sound.hpp"
#include "Music.hpp"

class AudioPlayer
{

public:
    enum PlayMusicMethod {
        InfiniteLoop = -1
    };
    
    AudioPlayer();
    ~AudioPlayer();

    // Hatls music and effects
    void StopAll();

    void ToggleSoundMute();
    int soundVolume() const;
    void soundVolume(float volumePercent);
    
    void ToggleMusicMute();
    int musicVolume() const;
    void musicVolume(float volumePercent);
    void PlayMusic(Music * const pMusic, int fadeInMs = 0, int loops = PlayMusicMethod::InfiniteLoop);
    void StopMusic(int fadeOutMs = 0);
    
    
    void PlaySound(Sound * const pSound);
private:
    int mSavedSoundVolumePercent = 0;
    int mSavedMusicVolumePercent = 0;
    
    AudioPlayer(const AudioPlayer &) = delete;
    AudioPlayer& operator=(const AudioPlayer &) = delete;
};


#endif /* AudioPlayer_hpp */
