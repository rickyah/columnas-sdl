//
//  Logger.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 24/6/16.
//
//

#ifndef Logger_hpp
#define Logger_hpp

#include <string>


// This file includes a working implementation for logger for debug builds
// and a noop implementation in release
#include "LoggerImpl.hpp"

class Logger
{
    
public:
  

//        template<typename ...Args>
//    void cyclOps::Logger::info(char* szFormat, Args &&...args) {
//        std::string strOutput = this->summarizeVariableArguments(std::forward<Args>(args)...);
//        this->log("INFO: %s", strOutput.c_str());
//    }
    void Info(const char *fmt, ...) const
    {
        va_list newArgs;
        va_start( newArgs, fmt );
        
        mInstanceImpl.LogInfo(fmt, newArgs);
        
        va_end( newArgs );
    }
    
    void Error(const char *fmt, ...) const
    {
        va_list newArgs;
        va_start( newArgs, fmt );
        
        mInstanceImpl.LogError(fmt, newArgs);
        
        va_end( newArgs );
    }
    
    void Warn(const char *fmt, ...) const
    {
        va_list newArgs;
        va_start( newArgs, fmt );
        
        mInstanceImpl.LogWarn(fmt, newArgs);
        
        va_end( newArgs );
    }
    
    const static Logger log;
private:
    Logger() {};

    static LoggerImpl mInstanceImpl;
};


#endif /* Logger_hpp */
