//  Created by Ricardo Amores Hernández on 25/16.

#include "./helpers.h"
#import <Foundation/Foundation.h>

std::string getResourceBundlePath()
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    
    if (mainBundle != nil)
    {
        NSString* bundlePath = [mainBundle resourcePath];
        return [bundlePath UTF8String];
    }
    
    return "";
}