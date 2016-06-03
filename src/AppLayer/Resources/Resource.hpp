//
//  Resource.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef Resource_hpp
#define Resource_hpp

#include <string>

#include "ResourceId.hpp"

/*
 * Base abstract class for all resources
 */
class Resource
{
public:
    uint32_t id() const { return mId; }
    std::string name() const {  return mName; };
    virtual void Load() = 0;
    
protected:
    Resource(ResourceId rId): mId(rId.id), mName(rId.name) {}
    virtual ~Resource() {};
private:
    Resource(const Resource &);
    Resource& operator=(const Resource &);
    
    uint32_t mId;
    std::string mName;
};



#endif /* Resource_hpp */
