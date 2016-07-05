//
//  LoggerImpl
//  king-test
//
//  Created by Ricardo Amores Hernández on 24/6/16.
//
//

#ifndef LoggerImpl_hpp
#define LoggerImpl_hpp

#include "Logger.hpp"

const Logger Logger::log = Logger();
LoggerImpl Logger::mInstanceImpl = LoggerImpl();

#endif /* LoggerImpl_hpp */
