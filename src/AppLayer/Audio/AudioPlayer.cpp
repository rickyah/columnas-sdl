//
//  AudioPlayer.cpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 23/6/16.
//
//

#include "AudioPlayer.hpp"

AudioPlayer::AudioPlayer()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(8);
}

AudioPlayer::~AudioPlayer()
{
    StopAll();
    Mix_CloseAudio();
}

void AudioPlayer::StopAll()
{
    // -1 to stop all channels
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
}

void AudioPlayer::PlaySound(Sound* const pSound)
{
    if (!pSound) return;
    
    Mix_PlayChannel(-1, pSound->pMixSound.get(), 0);
}


void AudioPlayer::PlayMusic(Music *const pMusic, int fadeInMs, int loops)
{
    if (!pMusic) return;
    
    Mix_FadeInMusic(pMusic->pMixMusic.get(), loops, fadeInMs);
}

void AudioPlayer::StopMusic(int fadeOutMs)
{
    Mix_FadeOutMusic(fadeOutMs);
}

void AudioPlayer::ToggleMusicMute()
{
    auto previousValue = musicVolume();
    musicVolume(mSavedMusicVolumePercent);
    mSavedMusicVolumePercent = previousValue;
}

int AudioPlayer::musicVolume() const
{
    return Mix_VolumeMusic(-1);
}

void AudioPlayer::musicVolume(float volumePercent)
{
    if (volumePercent < 0) volumePercent = 0;
    if (volumePercent > 1) volumePercent = 1;
    
    Mix_VolumeMusic(volumePercent * MIX_MAX_VOLUME);
}

void AudioPlayer::ToggleSoundMute()
{
    auto previousValue = soundVolume();
    soundVolume(mSavedSoundVolumePercent);
    mSavedSoundVolumePercent = previousValue;
}


int AudioPlayer::soundVolume() const
{
    return Mix_Volume(-1,-1);
}

void AudioPlayer::soundVolume(float volumePercent)
{
    if (volumePercent < 0) volumePercent = 0;
    if (volumePercent > 1) volumePercent = 1;
    
    Mix_Volume(-1, volumePercent * MIX_MAX_VOLUME);
}


