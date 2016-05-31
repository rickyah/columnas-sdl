//
//  ResourceId.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceId_hpp
#define ResourceId_hpp

#include "string"
#include "crc32.hpp"

// Defines a data type for a resource id. It contains
// name: full path of the file
// id:  crc32 hash of the name, used for lookup
struct ResourceId
{
    constexpr ResourceId(const char* _name, uint32_t _id):name(_name),id(_id) {}
    
    const char* name;
    const uint32_t id;
};

// Create a ResourceId structure given a name
constexpr ResourceId RESOURCE_ID(const char* str) {
    return ResourceId(str, crc32(str));
}


#endif /* ResourceId_hpp */
