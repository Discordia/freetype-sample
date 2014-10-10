#pragma once

#include <string>

using std::string;

#if defined(__ANDROID__)

#include <android/log.h>

enum LogLevel
{
    LOG_ERROR = ANDROID_LOG_ERROR,
    LOG_WARN = ANDROID_LOG_WARN,
    LOG_INFO = ANDROID_LOG_INFO,
    LOG_DEBUG = ANDROID_LOG_DEBUG,
    LOG_VERBOSE = ANDROID_LOG_VERBOSE
};

#else

enum LogLevel
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE
};

#endif

void logf(LogLevel level, const string tag, string format, ...);

#define LOGE(...) logf(LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) logf(LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGI(...) logf(LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) logf(LOG_DEBUG, LOG_TAG, __VA_ARGS__)