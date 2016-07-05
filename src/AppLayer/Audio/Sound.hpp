//
//  Sound.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 23/6/16.
//
//

#ifndef Sound_hpp
#define Sound_hpp

#include <memory>
#include <SDL2_mixer/SDL_mixer.h>

class Sound
{
    friend class AudioPlayer;
public:
    explicit Sound(Mix_Chunk* ptrChunk):pMixSound(std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk *)>(ptrChunk, Mix_FreeChunk)){}
    
private:
    
    std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk *)> pMixSound;
};

#endif /* Sound_hpp */
