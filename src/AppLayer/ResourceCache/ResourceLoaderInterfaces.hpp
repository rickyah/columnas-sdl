//
//  ResourceLoaderInterfaces.hpp
//
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceLoaderInterfaces_hpp
#define ResourceLoaderInterfaces_hpp

#include "Texture2d.hpp"
#include "Sound.hpp"
#include "Music.hpp"

class ITextureLoader
{
public:
    virtual ~ITextureLoader(){};
    virtual Texture2d* LoadResourceData(const std::string &resourcePath) = 0;
};


class ISoundLoader
{
public:
    virtual ~ISoundLoader(){};
    virtual Sound* LoadResourceData(const std::string &resourcePath) = 0;
};


class IMusicLoader
{
public:
    virtual ~IMusicLoader(){};
    virtual Music* LoadResourceData(const std::string &resourcePath) = 0;
};

#endif /* ResourceLoaderInterfaces_hpp */
