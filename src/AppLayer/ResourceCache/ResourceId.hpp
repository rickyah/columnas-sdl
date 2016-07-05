//
//  ResourceId.hpp
//
//
//  Created by Ricardo Amores Hernández on 31/5/16.
//
//

#ifndef ResourceId_hpp
#define ResourceId_hpp

#include "string"
#include "crc32.hpp"

/* Defines a data type for a resource id. It contains
 *
 * name:    full path of the resource, including the extension
 * id:      crc32 hash of the name, used for lookup
 */
struct ResourceId
{
    constexpr ResourceId(const char* Name, uint32_t Id):name(Name),id(Id) {}
    
    const char* name;
    const uint32_t id;
};

// Create a ResourceId structure given a string
constexpr ResourceId RESOURCE_ID(const char* str) {
    return ResourceId(str, crc32(str));
}


#endif /* ResourceId_hpp */
