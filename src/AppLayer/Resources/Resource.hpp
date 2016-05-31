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
    uint32_t id() const { return _id; }
    std::string name() const {  return _name; };
    virtual void Load() = 0;
    
protected:
    Resource(ResourceId rId):_id(rId.id), _name(rId.name) {}
    virtual ~Resource() {};
private:
    Resource(const Resource &);
    Resource& operator=(const Resource &);
    
    uint32_t _id;
    std::string _name;
};



#endif /* Resource_hpp */
