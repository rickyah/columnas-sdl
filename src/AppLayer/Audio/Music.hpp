//
//  Music.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 23/6/16.
//
//

#ifndef Music_hpp
#define Music_hpp

#include <memory>
#include <SDL2_mixer/SDL_mixer.h>

class Music
{
    friend class AudioPlayer;
public:
    Music(Mix_Music *ptrMusic):pMixMusic(std::unique_ptr<Mix_Music, void(*)(Mix_Music*)>(ptrMusic, Mix_FreeMusic)){}
    
private:
    
    std::unique_ptr<Mix_Music, void(*)(Mix_Music *)> pMixMusic;
};


#endif /* Music_hpp */
