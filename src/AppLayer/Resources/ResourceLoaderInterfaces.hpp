//
//  ResourceLoaderInterfaces.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceLoaderInterfaces_hpp
#define ResourceLoaderInterfaces_hpp

#include "Texture2d.hpp"

class ITextureLoader
{
public:
    virtual std::shared_ptr<Texture2d> LoadResourceData(const std::string &resourcePath) = 0;
};

#endif /* ResourceLoaderInterfaces_hpp */
