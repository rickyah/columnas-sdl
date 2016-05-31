//
//  ResourcesDefinitions.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 1/6/16.
//
//

#ifndef ResourcesDefinitions_hpp
#define ResourcesDefinitions_hpp

#include "Resource.hpp"
#include "ResourceLoaderInterfaces.hpp"
#include "Texture2d.hpp"

class Texture2dResource : public Resource
{
public:
    
    Texture2dResource(ResourceId rId, std::shared_ptr<ITextureLoader> pLoader):Resource(rId), _pResourceLoader(pLoader) {}
    
    std::shared_ptr<Texture2d> texture()
    {
        if (!_pResource)
        {
            Load();
        }
        
        return _pResource;
    }
    
    virtual void Load()
    {
        _pResource = _pResourceLoader->LoadResourceData(name());
    }
    
protected:
    
private:
    std::shared_ptr<Texture2d> _pResource;
    std::shared_ptr<ITextureLoader> _pResourceLoader;
};


#endif /* ResourcesDefinition_hpps */
