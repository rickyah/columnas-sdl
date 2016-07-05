//
//  LoggerImpl.hpp
//  king-test
//
//  Created by Ricardo Amores Hernández on 24/6/16.
//
//


#if DEBUG

#include <SDL2/SDL_log.h>
struct LoggerImpl
{
    void LogInfo(const char *fmt, va_list args) const
    {
        SDL_LogMessageV(0, SDL_LogPriority::SDL_LOG_PRIORITY_INFO, fmt, args);
    }
    
    void LogError(const char *fmt, va_list args) const
    {
        SDL_LogMessageV(0, SDL_LogPriority::SDL_LOG_PRIORITY_ERROR, fmt, args);
    }
    
    void LogWarn(const char *fmt, va_list args) const
    {
        SDL_LogMessageV(0, SDL_LogPriority::SDL_LOG_PRIORITY_WARN, fmt, args);
    }
    
};
#else

// Noop
struct LoggerImpl
{
    void LogInfo(const char *fmt, va_list args) const {}
    void LogError(const char *fmt, va_list args) const {}
    void LogWarn(const char *fmt, va_list args) const {}
};

#endif

